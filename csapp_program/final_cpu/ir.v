// ir 指令寄存器
module ir(
    clk_im_n, IR_Write, inst_code, inst,
    rst_n
);

    input clk_im_n;
    input IR_Write;
    input rst_n;
    input [31:0] inst_code; 
    output [31:0] inst;

    wire ir_clk;

    assign ir_clk = clk_im_n & IR_Write;

    reg_b32 r32(
        .clk(ir_clk),
        .rst_n(rst_n),
        .d(inst_code),
        .q(inst)
    );

endmodule
