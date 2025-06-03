module pc(
    PC_Write, clk_im_n, PC_o, PC_i,
    rst_n
);

    input PC_Write;
    input clk_im_n;
    input rst_n;
    input [31:0] PC_i;
    output [31:0] PC_o;

    wire pc_clk;
    
    assign pc_clk = clk_im_n & PC_Write;

    reg_b32 r32(
        .clk(pc_clk),
        .rst_n(rst_n),
        .d(PC_i),
        .q(PC_o)
    );

endmodule
