`timescale 1ns / 1ps
module board_top(
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


multi_alu alu_lab (
    .clk(clk),
    .rst_n(swb[4]),
    .clk_A(swb[1]),
    .clk_B(swb[2]),
    .clk_F(swb[3]),
    .sw(sw),
    .FR(led[1:4]),
    .seg(seg),
    .which(which)
);

endmodule