`timescale 1ns/1ps

module display_test();
    reg clk = 0;
    reg[31:0] data;
    wire[2:0] which;
    wire[7:0] seg;、
    wire[14:0] count;
    wire[3:0] digit;


    display display_instance (
        .clk(clk),
        .data(data),
        .which(which),
        .seg(seg),
        .count(count),
        .digit(digit)
    );

    always #0.01 clk = ~clk; 

    initial begin
        data = 32'hfedc_ba98; #500;
        data = 32'h7654_3210; #500;
    end
endmodule