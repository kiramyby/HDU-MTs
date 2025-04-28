module multi_alu (
    input  wire        clk_A,
    input  wire        clk_B,
    input  wire        clk_F,
    input  wire        rst_n,   
    input  wire [3:0]  ALU_OP,      
    input  wire [31:0] ALU_A,       
    input  wire [31:0] ALU_B,       
    output reg  [31:0] ALU_F,
    output reg  [3:0]  ALU_FR
);

alu ALU (
    .clk_A(clk_A),
    .clk_B(clk_B),
    .clk_F(clk_F),
    .ALU_OP(ALU_OP),
    .ALU_A(ALU_A),
    .ALU_B(ALU_B),
    .ALU_F(ALU_f)
);

reg_b32 A (
    .clk(clk_A),
    .rst_n(rst_n),
    .p(ALU_A),
    .q(ALU_A)
);
reg_b32 B (
    .clk(clk_B),
    .rst_n(rst_n),
    .p(ALU_B),
    .q(ALU_B)
);
reg_b32 F (
    .clk(clk_F),
    .rst_n(rst_n),
    .p(ALU_f),
    .q(ALU_F)
);

reg_b4 FR (
    .clk(clk_F),
    .rst_n(rst_n),
    .p(ALU_OP),
    .q(ALU_FR)
);

endmodule