module multi_alu (
    clk, clk_A, clk_B, clk_F, rst_n,
    sw, FR, seg, which
);

input         clk;
input         clk_A;
input         clk_B;
input         clk_F;
input         rst_n;    
input  [31:0] sw;

// FLAG LED显示
output [3:0]  FR;

// Result 数码管显示
output [7:0] seg;
output [2:0] which;

wire [31:0] alu_a;
wire [31:0] alu_b;
wire [31:0] alu_f;
wire [3:0]  alu_op = sw[3:0];
wire [3:0]  alu_fr;

wire [31:0] F;

alu ALU (
    .ALU_OP(alu_op),
    .ALU_A(alu_a),
    .ALU_B(alu_b),
    .ZF(alu_fr[3]),
    .SF(alu_fr[2]),
    .OF(alu_fr[1]),
    .CF(alu_fr[0]),
    .ALU_F(alu_f)
);

reg_b32 A (
    .clk(clk_A),
    .rst_n(rst_n),
    .d(sw),
    .q(alu_a)
);
reg_b32 B (
    .clk(clk_B),
    .rst_n(rst_n),
    .d(sw),
    .q(alu_b)
);
reg_b32 result_reg (
    .clk(clk_F),
    .rst_n(rst_n),
    .d(alu_f),
    .q(F)
);

reg_b4 flag_reg (
    .clk(clk_F),
    .rst_n(rst_n),
    .d(alu_fr),
    .q(FR)
);

display display_inst (
    .clk(clk),
    .data(F),
    .which(which),
    .seg(seg)
);

endmodule