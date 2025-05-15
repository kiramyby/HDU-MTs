module reg_file32_top (
    clk, Reg_Write, clk_RR, clk_F, clk_WB , W_Addr, ALU_OP,
    R_Addr_A, R_Addr_B ,FR , seg, which, rst_n,
    cus_enable, cus
);

input         clk;
input         Reg_Write;
input         clk_F;
input         clk_WB;
input         clk_RR;
input         rst_n;
input         cus_enable;
input  [3:0]  cus;
input  [3:0]  ALU_OP;
input  [4:0]  W_Addr;
input  [4:0]  R_Addr_A;
input  [4:0]  R_Addr_B;

// FLAG LED显示
output [3:0]  FR;

// Result 数码管显示
output [7:0] seg;
output [2:0] which;

// reg_file32_inst -> A, B
wire [31:0] reg_a;
wire [31:0] reg_b;
// A, B -> ALU
wire [31:0] alu_a;
wire [31:0] alu_b;
// ALU -> F, FR
wire [31:0] alu_f;
wire [3:0]  alu_fr;

wire [31:0] F;
reg [31:0] F_;

always @(*) begin 
    if ( cus_enable ) begin
         F_[3:0] = cus;
    end
end

// 寄存器堆， 用于ALU数据输入
reg_file32 reg_file32_inst (
    .Reg_Write(Reg_Write),
    .R_Addr_A(R_Addr_A),
    .R_Addr_B(R_Addr_B),
    .W_Addr(W_Addr),
    .W_Data(F_),
    .R_Data_A(reg_a),
    .R_Data_B(reg_b),
    .clk_Regs(clk_WB),
    .rst_n(rst_n)
);

alu ALU (
    .ALU_OP(ALU_OP),
    .ALU_A(alu_a),
    .ALU_B(alu_b),
    .ZF(alu_fr[3]),
    .SF(alu_fr[2]),
    .OF(alu_fr[1]),
    .CF(alu_fr[0]),
    .ALU_F(alu_f)
);

reg_b32 A (
    .clk(clk_RR),
    .rst_n(rst_n),
    .d(reg_a),
    .q(alu_a)
);
reg_b32 B (
    .clk(clk_RR),
    .rst_n(rst_n),
    .d(reg_b),
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