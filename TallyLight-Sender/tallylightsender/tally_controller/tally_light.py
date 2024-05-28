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
            ip (str): IP address of tally
            port (int): Port which tally listens on
            index (int): Tally TSL index
            enableFrontLight (bool, optional): Enable or disable front light. Defaults to True.
            brightness (Brightness, optional): Set brightness. Defaults to Brightness.MAX.
        """
        self._ip = ip
        self._port = port
        self._index = bytearray(index.to_bytes(2, "little"))
        self._enableFrontLight = enable_front_light
        self._brightness = brightness

    def set_color(self, color: Color) -> None:
        """Update Tally color

        Args:
            color (Color):
        """
        self._color = color
        print(
            f"Tally index: {int.from_bytes(self._index, 'little')}  Color: {self._color}"
        )

        # prepare package
        control = int(color)  # RH Tally Lamp state
        if self._enable_front_light:
            control = (int(color) << 2) | control  # Text Tally state
        control = (int(self._brightness) << 6) | control  # Brightness value
        self._control = bytearray(control.to_bytes(2, "little"))

        self._send_UDP_package()
        self._send_UDP_package()

    def _send_UDP_package(self) -> None:
        package = (
            self._PACKAGE_SIZE
            + self._VERSION
            + self._FLAGS
            + self._SCREEN
            + self._index
            + self._control
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

    _index = bytearray()  # Address of tally (see tsl docs)
    _control = bytearray()  # Light state (see tsl docs)

    _brightness: Brightness
    _color: Color = Color.OFF
    _enable_front_light: bool

    # Connection credentials
    _ip: str
    _port: int
