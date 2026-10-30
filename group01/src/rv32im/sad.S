# SAD baseline for RV32IM (CP2)
#   a0 = a0 + |a1 - a2|     signed compare, result wraps modulo 2^32
#
# Interface (same as README.md):
#   a0 : rd_old on entry, rd_new on exit
#   a1 : rs1        a2 : rs2
# Clobbers t0, t1, t2.
# Only RV32I base instructions: no pseudo-instructions, no memory access.

        .text
        .globl  sad_accumulate

sad_accumulate:
        # ---- kernel: 6 static instructions ----
        slt     t0, a1, a2              # t0 = 1 when a1 < a2 (signed)
        sub     t1, a1, a2              # a1 - a2
        sub     t2, a2, a1              # a2 - a1
        beq     t0, zero, use_t1        # a1 >= a2: keep a1 - a2
        add     t1, t2, zero            # a1 < a2: t1 = a2 - a1
use_t1:
        add     a0, a0, t1              # accumulator + absolute difference

        # ---- return to caller (call overhead, not part of the kernel) ----
        jalr    zero, 0(ra)             # same as the pseudo-instruction "ret"
