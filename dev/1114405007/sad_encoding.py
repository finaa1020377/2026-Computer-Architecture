"""Generate example machine-code encodings for the custom SAD instruction."""


def sad_machine_code(
    rd: int,
    rs1: int,
    rs2: int,
    funct7: int = 0,
    funct3: int = 0,
) -> int:
    """Return a 32-bit SAD encoding; funct fields default to examples only."""
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

    opcode = 0b0001011
    return (
        (funct7 << 25)
        | (rs2 << 20)
        | (rs1 << 15)
        | (funct3 << 12)
        | (rd << 7)
        | opcode
    )


def main() -> None:
    examples = (
        ("SAD x5, x10, x13", 5, 10, 13, 0x00D5028B),
        ("SAD x0, x5, x5", 0, 5, 5, 0x0052800B),
        ("SAD x8, x20, x30", 8, 20, 30, 0x01EA040B),
    )
    for instruction, rd, rs1, rs2, expected in examples:
        encoding = sad_machine_code(rd, rs1, rs2)
        if encoding != expected:
            raise AssertionError(
                f"{instruction}: expected 0x{expected:08X}, "
                f"got 0x{encoding:08X}"
            )
        print(f"{instruction}: 0x{encoding:08X}")


if __name__ == "__main__":
    main()
