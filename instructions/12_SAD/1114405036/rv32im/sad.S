# SAD baseline for RV32IM (CP2)
#   a0 = a0 + |a1 - a2|
#   Wrap semantics: the subtraction, the absolute value and the
#   accumulation all keep the low 32 bits (modulo 2^32).
#
# Interface (same as README.md):
#   a0 : rd_old on entry, rd_new on exit
#   a1 : rs1        a2 : rs2
# Clobbers t1, t2.
# Only RV32I base instructions: no pseudo-instructions, no memory access.

        .text
        .globl  sad_accumulate

sad_accumulate:
        # ---- kernel: 5 static instructions, no branch ----
        sub     t1, a1, a2              # t1 = rs1 - rs2 (mod 2^32)
        srai    t2, t1, 31              # t2 = 0 if t1 >= 0, 0xFFFFFFFF if t1 < 0
        xor     t1, t1, t2              # t1 = t1 or ~t1, depending on the sign
        sub     t1, t1, t2              # t1 = |rs1 - rs2| (mod 2^32)
        add     a0, a0, t1              # rd_old + |d| (mod 2^32)

        # ---- return to caller (call overhead, not part of the kernel) ----
        jalr    zero, 0(ra)             # same as the pseudo-instruction "ret"
