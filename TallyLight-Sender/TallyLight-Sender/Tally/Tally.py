import socket
from TallyEnums import Brightness, Color


class Tally:

    def __init__(
        self,
        IP: str,
        port: int,
        index: int,
        enableFrontLight: bool = True,
        brightness: Brightness = Brightness.MAX,
    ):
        self.__IP = IP
        self.__port = port
        self.__index = index.to_bytes(2, "little")
        self.__enableFrontLight = enableFrontLight
        self.__brightness = brightness

    def SetColor(self, color: Color) -> None:
        self.__color = color
        print(
            f"Tally index: {int.from_bytes(self.__index, 'little')}  Color: {self.__color}"
        )

        #prepare package
        control = int(color)  # RH Tally Lamp state
        if self.__enableFrontLight:
            control = (int(color) << 2) | control  # Text Tally state
        control = (int(self.__brightness) << 6) | control  # Brightness value
        self.__control = control.to_bytes(2, "little")

        self.__SendUDPPackage()
        self.__SendUDPPackage()

    def __SendUDPPackage(self) -> None:
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
    __enableFrontLight: bool

    # Connection credentials
    __IP: str
    __port: int
