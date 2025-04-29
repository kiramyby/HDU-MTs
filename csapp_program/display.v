`timescale 1ns/1ps

module display ( clk, data, which, seg, count, digit);
    input clk;
    input [32:1] data;
    output reg [2:0] which=0;
    output reg [7:0] seg;

    output reg [14:0] count =  0;
    always @(posedge clk)
        count <= count + 1'b1;
    