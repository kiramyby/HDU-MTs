module test;
	// Inputs
	reg clk;
	reg rst;
	reg clk_M;
	// Outputs
	wire OF;
	wire ZF;
	wire [31:0] F;
	wire [2:0] ALU_OP;
	wire [31:0] M_R_Data;
	wire [1:0] w_r_s;
	wire imm_s;
	wire rt_imm_s;
	wire Mem_Write;
	wire Write_Reg;
	wire [31:0] PC;
	wire [1:0] PC_s;
	wire [31:0] R_Data_B;
	wire [31:0] Inst_code;
	CPU uut (
		.clk(clk), 
		.rst(rst), 
		.OF(OF), 
		.ZF(ZF), 
		.F(F), 
		.ALU_OP(ALU_OP), 
		.M_R_Data(M_R_Data), 
		.w_r_s(w_r_s), 
		.imm_s(imm_s), 
		.rt_imm_s(rt_imm_s), 
		.Mem_Write(Mem_Write), 
		.Write_Reg(Write_Reg), 
		.PC(PC), 
		.PC_s(PC_s), 
		.clk_M(clk_M), 
		.R_Data_B(R_Data_B), 
		.Inst_code(Inst_code)
	);
	always #4  clk_M = ~clk_M;
	always #16  clk =~clk;
	initial begin
		clk = 0;
		rst = 1;
		clk_M = 0;
		#2;
		rst = 0;
	end
endmodule