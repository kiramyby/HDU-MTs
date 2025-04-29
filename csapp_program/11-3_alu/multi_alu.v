module multi_alu (
    input  wire        clk,
    input  wire        clk_A,
    input  wire        clk_B,
    input  wire        clk_F,
    input  wire        rst_n,    
    input  wire [31:0] sw,

    // Flag, LED display
    output reg  [3:0]  FR,

    // Result, seg digit display
    output wire [7:0] SEG,
    output wire [2:0] WHICH
);

wire [31:0] alu_a;
wire [31:0] alu_b;
wire [31:0] alu_f;
wire [3:0]  alu_op = sw[31:28];
wire [3:0]  alu_fr;

reg  [31:0] F;

alu ALU (
    .clk_A(clk_A),
    .clk_B(clk_B),
    .clk_F(clk_F),
    .ALU_OP(alu_op),
    .ALU_A(alu_a),
    .ALU_B(alu_b),
    .ZF(alu_fr[3]),
    .SF(alu_fr[2]),
    .OF(alu_fr[1]),
    .CF(alu_fr[0]),
    .ALU_F(alu_f),
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
reg_b32 F (
    .clk(clk_F),
    .rst_n(rst_n),
    .d(alu_f),
    .q(F)
);

reg_b4 FR (
    .clk(clk_F),
    .rst_n(rst_n),
    .d(alu_fr),
    .q(FR)
);

display display_inst (
    .clk(clk),
    .data(ALU_F),
    .which(WHICH),
    .seg(SEG)
);

endmodule