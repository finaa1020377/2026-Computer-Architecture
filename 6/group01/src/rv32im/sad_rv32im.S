    .text
    .align 2
    .globl sad_accumulate_rv32im
    .type sad_accumulate_rv32im, @function

# uint32_t sad_accumulate_rv32im(uint32_t acc, int32_t rs1, int32_t rs2)
# a0 = acc (in/out), a1 = rs1, a2 = rs2
sad_accumulate_rv32im:
    slt   t0, a1, a2        # t0 = 1 when a1 < a2 (signed compare)
    sub   t1, a1, a2        # t1 = rs1 - rs2
    sub   t2, a2, a1        # t2 = rs2 - rs1
    beq   t0, zero, 1f      # if rs1 >= rs2, keep t1
    add   t1, t2, zero      # else select t2 without pseudo-instruction
1:
    add   a0, a0, t1        # acc + abs(rs1 - rs2), wraps in 32-bit
    jalr  zero, 0(ra)

    .size sad_accumulate_rv32im, .-sad_accumulate_rv32im
