import socket

from tally_controller.tally_light_enums import Brightness, Color


class Tally:
    """Tally Light representation"""

    def __init__(
        self,
        ip: str,
        port: int,
        index: int,
        enable_front_light: bool = True,
        brightness: Brightness = Brightness.MAX,
    ):
        """

        Args:
            ip (str): Tally light IP address
            port (int): Port which tally listens on
            index (int): Tally TSL index
            enable_front_light (bool, optional): Enable or disable front light. Defaults to True.
            brightness (Brightness, optional): Set brightness. Defaults to Brightness.MAX.
        """
        self._ip = ip
        self._port = port
        self._index = bytearray(index.to_bytes(2, "little"))
        self.enable_front_light = enable_front_light
        self.brightness = brightness
        self.color = Color.OFF

    def set_color(self, color: Color) -> None:
        """Update Tally color

        Args:
            color (Color):
        """
        self.color = color
        print(
            f"Tally index: {int.from_bytes(self._index, 'little')}  Color: {self.color}"
        )

        # prepare package
        control = int(color)  # RH Tally Lamp state
        if self.enable_front_light:
            control = (int(color) << 2) | control  # Text Tally state
        control = (int(self.brightness) << 6) | control  # Brightness value
        control_bin = bytearray(control.to_bytes(2, "little"))

        self._send_UDP_package(control_bin)
        self._send_UDP_package(control_bin)

    def _send_UDP_package(self, control: bytearray) -> None:
        package = (
            self._PACKAGE_SIZE
            + self._VERSION
            + self._FLAGS
            + self._SCREEN
            + self._index
            + control
            + self._LENGTH
        )

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        res = sock.sendto(package, (self._ip, self._port))
        if res == -1:
            print(
                f"Error during sending :(    Index: {int.from_bytes(self._index, 'little')}, IP: {self._ip}:{self._port}"
            )

    # Constants (see tsl docs)
    _PACKAGE_SIZE = int(10).to_bytes(2, "little")
    _VERSION = bytearray([0])
    _FLAGS = bytearray([0])
    _SCREEN = bytearray([0, 0])
    _LENGTH = bytearray([0, 0])
