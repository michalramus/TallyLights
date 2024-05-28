from enum import IntEnum


class Color(IntEnum):
    """Tally Lights colors"""

    OFF = 0
    RED = 1
    GREEN = 2


class Brightness(IntEnum):
    """Tally Lights brightness"""

    LOW = 0
    MEDIUM = 1
    HIGH = 2
    MAX = 3
