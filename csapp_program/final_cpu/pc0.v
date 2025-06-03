module pc0(
    PC_Write, clk_im_n, PC0_o, PC0_i,
    rst_n
);

    input PC_Write;
    input clk_im_n;
    input rst_n;
    input [31:0] PC0_i;
    output [31:0] PC0_o;

    wire pc_clk;
    
    assign pc_clk = clk_im_n & PC_Write;

    reg_b32 r32(
        .clk(pc_clk),
        .rst_n(rst_n),
        .d(PC0_i),
        .q(PC0_o)
    );

endmodule