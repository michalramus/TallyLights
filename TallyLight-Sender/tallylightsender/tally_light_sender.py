import obsws_python as obs
from tally_controller import tally_light, tally_light_enums
import time

host = "192.168.200.2"
port = 4455
password = "123456"
timeout = 3

cr: obs.ReqClient
ce: obs.EventClient

global tallies
tallies: list[tuple[str, tally_light.Tally]]  # [Scene name, Tally]

global previewTally
previewTally: tuple[str, tally_light.Tally] | None = None  # Scene on preview
global programTally
programTally: tuple[str, tally_light.Tally] | None = None  # Scene on program


def main():
    # from Tally import Tally  # I don't know why, but without it doesn't work

    print(f"Host: {host}:{port}")
    print("Connecting...")

    global tallies
    tallies = [
        ("cam1", tally_light.Tally("192.168.200.11", 10373, 1)),
        ("cam2", tally_light.Tally("192.168.200.12", 10373, 2)),
        ("cam3", tally_light.Tally("192.168.200.13", 10373, 3)),
    ]

    while True:  # Retry until connected
        try:
            global ce
            ce = obs.EventClient(
                host=host, port=port, password=password, timeout=timeout
            )
            global cr
            cr = obs.ReqClient(host=host, port=port, password=password, timeout=timeout)

        except Exception as e:
            time.sleep(timeout)
        else:
            break

    # Set previewTally and programTally variables
    global programTally
    resp = cr.get_current_program_scene()
    scene_name = resp.scene_name
    for tally in tallies:
        if tally[0] == scene_name:
            programTally = tally
            break

    global previewTally
    resp = cr.get_current_preview_scene()
    scene_name = resp.scene_name
    for tally in tallies:
        if tally[0] == scene_name:

            previewTally = tally
            break

    # Register callbacks
    ce.callback.register(on_scene_transition_started)
    ce.callback.register(on_scene_transition_ended)
    print("Connected 😄")


def on_scene_transition_started(data):
    print("Transition stared")
    resp = cr.get_current_program_scene()
    scene_name = resp.scene_name

    for tally in tallies:
        if tally[0] == scene_name:
            tally[1].set_color(tally_light_enums.Color.RED)
            break


def on_scene_transition_ended(data):
    print("Transition ended")
    global previewTally
    global programTally

    resp = cr.get_current_program_scene()
    scene_name = resp.scene_name
    found = False
    for tally in tallies:
        if tally[0] == scene_name:
            # Third scene goes to program; Second goes from program to preview; First scene is turned off from preview
            if (previewTally is not None) and (tally[0] != previewTally[0]):
                previewTally[1].set_color(tally_light_enums.Color.OFF)

            # Swap program and preview
            previewTally = programTally
            programTally = tally
            if (previewTally is not None) and (previewTally != programTally):
                previewTally[1].set_color(tally_light_enums.Color.OFF)

            found = True
            break
    if (not found) and (programTally is not None):
        programTally[1].set_color(tally_light_enums.Color.OFF)
        programTally = None


if __name__ == "__main__":
    main()
    while True:
        time.sleep(30)
        try:
            cr.get_current_program_scene()  # check if connected
        except:
            print("Disconnected")
            main()
