module pc(
    PC_Write, clk_im_n, IM_Addr, IM_Addr_nx,
    rst_n
);

    input PC_Write;
    input clk_im_n;
    input rst_n;
    input [31:0] IM_Addr_nx;
    output [31:0] IM_Addr;

    wire pc_clk;
    
    // 时钟门控逻辑：只有当PC_Write为高时才允许时钟通过
    assign pc_clk = clk_im_n & PC_Write;

    reg_b32 r32(
        .clk(pc_clk),
        .rst_n(rst_n),
        .d(IM_Addr_nx),
        .q(IM_Addr)
    );

endmodule