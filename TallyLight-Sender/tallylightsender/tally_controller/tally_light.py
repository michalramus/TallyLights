import socket
from tally_controller.tally_light_enums import Brightness, Color


class Tally:
    """Tally Light representation
    """

    def __init__(
        self,
        IP: str,
        port: int,
        index: int,
        enable_front_light: bool = True,
        brightness: Brightness = Brightness.MAX,
    ):
        """_summary_

        Args:
            IP (str): IP address of tally
            port (int): Port which tally listens on
            index (int): Tally TSL index
            enableFrontLight (bool, optional): Enable or disable front light. Defaults to True.
            brightness (Brightness, optional): Set brightness. Defaults to Brightness.MAX.
        """
        self.__IP = IP
        self.__port = port
        self.__index = index.to_bytes(2, "little")
        self.__enableFrontLight = enable_front_light
        self.__brightness = brightness

    def set_color(self, color: Color) -> None:
        """Update Tally color

        Args:
            color (Color):
        """
        self.__color = color
        print(
            f"Tally index: {int.from_bytes(self.__index, 'little')}  Color: {self.__color}"
        )

        #prepare package
        control = int(color)  # RH Tally Lamp state
        if self.__enable_front_light:
            control = (int(color) << 2) | control  # Text Tally state
        control = (int(self.__brightness) << 6) | control  # Brightness value
        self.__control = control.to_bytes(2, "little")

        self.__send_UDP_package()
        self.__send_UDP_package()

    def __send_UDP_package(self) -> None:
        package = (
            self.__PACKAGE_SIZE
            + self.__VERSION
            + self.__FLAGS
            + self.__SCREEN
            + self.__index
            + self.__control
            + self.__LENGTH
        )

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        res = sock.sendto(package, (self.__IP, self.__port))
        if res == -1:
            print(
                f"Error during sending :(    Index: {int.from_bytes(self.__index, 'little')}, IP: {self.__IP}:{self.__port}"
            )

    # Constants (see tsl docs)
    __PACKAGE_SIZE = int(10).to_bytes(2, "little")
    __VERSION = bytearray([0])
    __FLAGS = bytearray([0])
    __SCREEN = bytearray([0, 0])
    __LENGTH = bytearray([0, 0])

    __index = bytearray()  # Address of tally (see tsl docs)
    __control = bytearray()  # Light state (see tsl docs)

    __brightness: Brightness
    __color: Color = Color.OFF
    __enable_front_light: bool

    # Connection credentials
    __IP: str
    __port: int
