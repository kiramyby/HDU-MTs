`timescale 1ns / 1ps
module board_top(
    clk, swb, sw,
    led, seg, which
);

input   wire          clk;
input   wire [4:1]    swb;
input   wire [31:0]   sw;
output  wire [3:0]    led;
output  wire [7:0]    seg;
output  wire [2:0]    which;

multi_alu alu_lab (
    .clk(clk),
    .rst_n(swb[4]),
    .clk_A(swb[1]),
    .clk_B(swb[2]),
    .clk_F(swb[3]),
    .sw(sw),
    .FR(led),
    .seg(seg),
    .which(which)
);

endmodule