module ir(
    clk_im, IR_Write, inst_code, inst,
    rst_n
);

    input clk_im;
    input IR_Write;
    input rst_n;
    input [31:0] inst_code; 
    output [31:0] inst;

    wire ir_clk;
    
    // 时钟门控逻辑：只有当IR_Write为高时才允许时钟通过
    assign ir_clk = clk_im & IR_Write;

    reg_b32 r32(
        .clk(ir_clk),
        .rst_n(rst_n),
        .d(inst_code),
        .q(inst)
    );

endmodule