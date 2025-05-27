`timescale 1ns / 1ps
module board_top__reg_file32(
    clk, swb, sw,
    led, seg, which, enable
);

input            clk;
input   [1:4]    swb;
input   [1:32]   sw;
output  [1:4]    led;
output  [7:0]    seg;
output  [2:0]    which;
output reg enable = 1;


reg_file32_top reg_file32_top_inst (
    .clk(clk),
    .clk_RR(swb[1]),
    .clk_F(swb[2]),
    .clk_WB(swb[3]),
    .W_Addr(sw[1:5]),
    .ALU_OP(sw[29:32]),
    .R_Addr_A(sw[6:10]),
    .R_Addr_B(sw[11:15]),
    .cus(sw[16:19]),
    .FR(led),
    .seg(seg),
    .which(which),
    .cus_enable(swb[4]),
    .Reg_Write(sw[28]),
    .rst_n(sw[27])
);

endmodule