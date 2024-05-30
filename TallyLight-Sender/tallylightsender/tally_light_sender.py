import time

import obsws_python as obsws
from tally_controller import tally_light, tally_light_enums


class OBS:

    def __init__(
        self,
        host: str,
        port: int,
        password: str,
        conn_timeout: int,
        tallies: list[tuple[str, tally_light.Tally]],
    ) -> None:
        """

        Args:
            host (str): IP Address of OBS
            port (int): Port which Websocket listens on
            password (str): OBS Websocket password
            conn_timeout (int): Connection timeout
            tallies (list[tuple[str, tally_light.Tally]]): List of tally lights
        """
        self._host = host
        self._port = port
        self._password = password
        self._conn_timeout = conn_timeout
        self._tallies = tallies

        self.connect()

    def connect(self) -> None:
        """Connect if disconnected

        Method blocks program execution until connected
        """
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
                    host=self._host, port=self._port, password=self._password, timeout=self._conn_timeout
                )

            except Exception:
                time.sleep(self._conn_timeout)
            else:
                connected = True

        # Register callbacks
        self._ce.callback.register(self.on_scene_transition_started)
        self._ce.callback.register(self.on_scene_transition_ended)
        # Update preview and program tally variables
        self._preview_tally = self._get_current_preview_tally()
        self._program_tally = self._get_current_program_tally()
        print("Connected 😄")

    def isConnected(self) -> bool:
        try:
            self._cr.get_version()  # check if connected
        except Exception:
            print("Disconnected")
            for tally in self._tallies:
                tally[1].set_color(tally_light_enums.Color.OFF)

            return False
        return True

    def _get_current_program_tally(self) -> tuple[str, tally_light.Tally] | None:
        """Fetch current program scene from OSB and match it to tally

        Returns:
            tuple[str, tally_light.Tally] | None: Tally of None if cannot match
        """
        resp = self._cr.get_current_program_scene()
        scene_name = resp.scene_name

        for tally in self._tallies:
            if tally[0] == scene_name:
                return tally
        return None

    def _get_current_preview_tally(self) -> tuple[str, tally_light.Tally] | None:
        """Fetch current preview scene from OSB and match it to tally

        Returns:
            tuple[str, tally_light.Tally] | None: Tally of None if cannot match
        """
        resp = self._cr.get_current_preview_scene()
        scene_name = resp.scene_name

        for tally in self._tallies:
            if tally[0] == scene_name:
                return tally
        return None

    def on_scene_transition_started(self, data):
        """Callback handler"""
        print("Transition stared")

        tally = self._get_current_program_tally()
        if tally is not None:
            tally[1].set_color(tally_light_enums.Color.RED)

    def on_scene_transition_ended(self, data):
        """Callback handler"""
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
    #TODO: Reading Tally configuration and OBS credentials from external file
    tallies = [
        ("cam1", tally_light.Tally("192.168.200.11", 10373, 1)),
        ("cam2", tally_light.Tally("192.168.200.12", 10373, 2)),
        ("cam3", tally_light.Tally("192.168.200.13", 10373, 3)),
    ]

    host = "192.168.200.2"
    port = 4455
    password = "123456"
    timeout = 3

    obs = OBS(host, port, password, timeout, tallies)

    while True:  # Check connection
        if not obs.isConnected():
            obs.connect()
        time.sleep(30)


if __name__ == "__main__":
    main()
