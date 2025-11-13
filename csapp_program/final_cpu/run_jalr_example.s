// 使用 JALR 替代 j L 的示例
main:
    addi    a0      zero,   0x10
    ori     a1,     zero,   3
    xori    a2,     zero,   0x30
    jal     BankSum
    lw      s0,     0(a2)
    add     zero,   zero,   zero
    add     zero,   zero,   zero
BankSum:
    add     t0,     a0,     zero
    or      t1,     a1,     zero
    and     t2,     zero,   zero
    auipc   t6,     0           // 获取当前PC到t6
    addi    t6,     t6,     -28  // 计算L标签的地址（向前28字节）
L:  lw      t3,     0(t0)
    add     t2,     t2,     t3
    addi    t0,     t0,     4
    addi    t1,     t1,     -1
    beq     t1,     zero,exit
    jalr    zero,   t6,     0    // 使用 jalr 跳转到 L，等效于原来的 j L
exit: sw    t2,0(a2)
      jr    ra
