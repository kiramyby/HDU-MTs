`timescale 1ns/1ps

module display_test();
    reg clk = 0;
    reg[31:0] data;
    wire[2:0] which;
    wire[7:0] seg;
    reg rst_n = 1; // 复位信号
    wire[14:0] count;
    wire[3:0] digit;


    display display_instance (
        .clk(clk),
        .rst_n(rst_n),
        .data(data),
        .which(which),
        .seg(seg),
        .count(count),
        .digit(digit)
    );

    always #0.01 clk = ~clk; 

    initial begin
        clk = 0;
        rst_n = 1;
        data = 32'hfedc_ba98; #500;
        data = 32'h7654_3210; #500;
        #5 rst_n = 0; // 复位
        #5 rst_n = 1; // 解除复位
        data = 32'hfedc_ba98; #500;
        data = 32'h7654_3210; #500;
    end
endmodule