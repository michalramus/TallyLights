import obsws_python as obsws

# from obsws_python.error import OBSSDKTimeoutError
from tally_controller import tally_light, tally_light_enums
import time

host = "192.168.200.2"
port = 4455
password = "123456"
timeout = 3


class OBS:

    def __init__(
        self,
        host: str,
        port: int,
        password: str,
        conn_timeout: int,
        tallies: list[tuple[str, tally_light.Tally]],
    ) -> None:
        self._host = host
        self._port = port
        self._password = password
        self._conn_timeout = conn_timeout
        self._tallies = tallies

        self.connect()

    def connect(self) -> None:
        print(f"Host: {self._host}:{self._port}")
        print("Connecting...")

        connected = False
        while not connected:  # Retry until connected
            try:
                self._ce = obsws.EventClient(
                    host=self._host,
                    port=self._port,
                    password=self._password,
                    timeout=self._conn_timeout,
                )
                self._cr = obsws.ReqClient(
                    host=host, port=port, password=password, timeout=timeout
                )

            except Exception:
                time.sleep(timeout)
            else:
                connected = True

        # Register callbacks
        self._ce.callback.register(self.on_scene_transition_started)
        self._ce.callback.register(self.on_scene_transition_ended)
        # Update preview and program tally variables
        self._preview_tally = self._get_current_preview_tally()
        self._program_tally = self._get_current_program_tally()
        print("Connected 😄")

    def _get_current_program_tally(self) -> tuple[str, tally_light.Tally] | None:
        resp = self._cr.get_current_program_scene()
        scene_name = resp.scene_name

        for tally in self._tallies:
            if tally[0] == scene_name:
                return tally
        return None

    def _get_current_preview_tally(self) -> tuple[str, tally_light.Tally] | None:
        resp = self._cr.get_current_preview_scene()
        scene_name = resp.scene_name

        for tally in self._tallies:
            if tally[0] == scene_name:
                return tally
        return None

    def on_scene_transition_started(self, data):
        print("Transition stared")

        tally = self._get_current_program_tally()
        if tally is not None:
            tally[1].set_color(tally_light_enums.Color.RED)

    def on_scene_transition_ended(self, data):
        print("Transition ended")

        tally = self._get_current_program_tally()
        if tally is not None:
            # Third scene goes to program; Second goes from program to preview; First scene is turned off from preview
            if (self._preview_tally is not None) and (
                tally[0] != self._preview_tally[0]
            ):
                self._preview_tally[1].set_color(tally_light_enums.Color.OFF)

            # Swap program and preview
            self._preview_tally = self._program_tally
            self._program_tally = tally
            if (self._preview_tally is not None) and (
                self._preview_tally != self._program_tally
            ):
                self._preview_tally[1].set_color(tally_light_enums.Color.OFF)

        elif (tally is None) and (self._program_tally is not None):
            self._program_tally[1].set_color(tally_light_enums.Color.OFF)
            self._program_tally = None


def main():
    tallies = [
        ("cam1", tally_light.Tally("192.168.200.11", 10373, 1)),
        ("cam2", tally_light.Tally("192.168.200.12", 10373, 2)),
        ("cam3", tally_light.Tally("192.168.200.13", 10373, 3)),
    ]

    x = OBS(host, port, password, timeout, tallies)


if __name__ == "__main__":
    main()
    while True:
        time.sleep(30)
        # try:
        #     cr.get_current_program_scene()  # check if connected
        # except OBSSDKError:
        #     print("Disconnected")
        #     main()
