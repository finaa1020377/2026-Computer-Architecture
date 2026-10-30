"""Generate machine-code encodings for the custom SAD instruction."""

OPCODE = 0b0001011  # custom-0
FUNCT7 = 0b0000000
FUNCT3 = 0b000


def sad_machine_code(
    rd: int,
    rs1: int,
    rs2: int,
    funct7: int = FUNCT7,
    funct3: int = FUNCT3,
) -> int:
    """Return the 32-bit R-type encoding of SAD rd, rs1, rs2."""
    fields = (
        ("rd", rd, 5),
        ("rs1", rs1, 5),
        ("rs2", rs2, 5),
        ("funct7", funct7, 7),
        ("funct3", funct3, 3),
    )
    for name, value, width in fields:
        if not isinstance(value, int) or isinstance(value, bool):
            raise TypeError(f"{name} must be an integer")
        if not 0 <= value < (1 << width):
            raise ValueError(f"{name} must fit in {width} bits")

    return (
        (funct7 << 25)
        | (rs2 << 20)
        | (rs1 << 15)
        | (funct3 << 12)
        | (rd << 7)
        | OPCODE
    )


def binary_fields(rd: int, rs1: int, rs2: int) -> str:
    """Return the encoding split as funct7|rs2|rs1|funct3|rd|opcode."""
    return " | ".join(
        (
            f"{FUNCT7:07b}",
            f"{rs2:05b}",
            f"{rs1:05b}",
            f"{FUNCT3:03b}",
            f"{rd:05b}",
            f"{OPCODE:07b}",
        )
    )


def main() -> None:
    examples = (
        ("SAD x10, x11, x12", 10, 11, 12, 0x00C5850B),
        ("SAD x5, x6, x7", 5, 6, 7, 0x0073028B),
        ("SAD x15, x16, x17", 15, 16, 17, 0x0118078B),
    )
    for instruction, rd, rs1, rs2, expected in examples:
        encoding = sad_machine_code(rd, rs1, rs2)
        if encoding != expected:
            raise AssertionError(
                f"{instruction}: expected 0x{expected:08X}, "
                f"got 0x{encoding:08X}"
            )
        print(f"{instruction}: 0x{encoding:08X}")
        print(f"  {binary_fields(rd, rs1, rs2)}")


if __name__ == "__main__":
    main()
