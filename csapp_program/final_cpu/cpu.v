module cpu(
    clk, rst_n
);

    input clk;
    input rst_n;

    // 信号声明
    wire [31:0] inst;
    wire [31:0] inst_code;
    wire [31:0] PC_o, PC_i, PC0_o;
    wire [31:0] ADD1_o, ADD2_o;
    wire [31:0] imm32;
    wire [4:0] rs1, rs2, rd;
    wire [6:0] opcode, funct7;
    wire [2:0] funct3;
    wire [31:0] reg_a, reg_b, alu_a, alu_b, Reg_B;
    wire [31:0] alu_f, F, F_;
    wire [3:0] alu_fr, FR, ALU_OP, ALU_OP_o;
    wire [31:0] M_R_Data, MDR_o;
    
    // 时钟信号
    assign clk_im = clk;
    
    // 控制信号
    wire PC_Write, PC0_Write, IR_Write, Reg_Write, Mem_Write;
    wire rs2_imm_s;
    wire [1:0] w_data_s, PC_s;
    wire IS_R, IS_IMM, IS_LUI, IS_LW, IS_SW, IS_BEQ, IS_JALR, IS_JAL;
    
    // 需要在always块中赋值的信号声明为reg
    reg [31:0] PC_i_reg, F_reg;
    
    // 连接reg到wire
    assign PC_i = PC_i_reg;
    assign F_ = F_reg;

    // 第一自增4
    assign ADD1_o = PC_o + 32'h00000004;

    // 第二合并
    assign ADD2_o = PC0_o + imm32;

    // 三选一选择器
    always @(*) begin
        case (PC_s)
            2'b00: PC_i_reg = ADD1_o; // PC0 + 4
            2'b01: PC_i_reg = ADD2_o; // PC0 + imm32
            2'b10: PC_i_reg = F;   // F alu运算结果
            default: PC_i_reg = ADD1_o;
        endcase
    end

    // 二选一选择器
    assign  alu_b = rs2_imm_s ? imm32 : Reg_B;

    // 四选一选择器
    always @(*) begin
        case (w_data_s)
            2'b00: F_reg = F; // alu运算结果
            2'b01: F_reg = imm32; // 立即数
            2'b10: F_reg = MDR_o; // 存储器数据
            2'b11: F_reg = ADD1_o; // PC+4 (for JAL/JALR)
            default: F_reg = F;
        endcase
    end

    pc PC(
        .PC_Write(PC_Write),
        .clk_im_n(~clk),
        .PC_o(PC_o),
        .PC_i(PC_i),
        .rst_n(rst_n)
    );

    pc0 PC0(
        .PC_Write(PC0_Write),
        .clk_im_n(~clk),
        .PC0_o(PC0_o),
        .PC0_i(PC_o),
        .rst_n(rst_n)
    );

    ir IR(
        .clk_im_n(~clk),
        .IR_Write(IR_Write),
        .inst_code(inst_code),
        .inst(inst),
        .rst_n(rst_n)
    );

    InstROM instROMInst(
        .clka(clk_im),
        .addra(PC_o[7:2]),
        .doubta(inst_code)
    );

    id1 ID1(
        .inst(inst),
        .rs1(rs1),
        .rs2(rs2),
        .rd(rd),
        .opcode(opcode),
        .funct3(funct3),
        .funct7(funct7),
        .imm32(imm32)
    );

    id2 ID2(
        .opcode(opcode),
        .funct3(funct3), 
        .funct7(funct7),
        .IS_R(IS_R),
        .IS_IMM(IS_IMM),
        .IS_LUI(IS_LUI),
        .IS_LW(IS_LW),
        .IS_SW(IS_SW),
        .IS_BEQ(IS_BEQ),
        .IS_JALR(IS_JALR),
        .IS_JAL(IS_JAL),
        .ALU_OP(ALU_OP)
    );

    cu CU(
        .IS_R(IS_R),
        .IS_IMM(IS_IMM),
        .IS_LUI(IS_LUI),
        .IS_LW(IS_LW),
        .IS_SW(IS_SW),
        .IS_BEQ(IS_BEQ),
        .IS_JALR(IS_JALR),
        .IS_JAL(IS_JAL),
        .clk(clk),
        .rst_n(rst_n),
        .ALU_OP(ALU_OP),
        .ZF_rs(FR[3]),
        .PC_Write(PC_Write),
        .PC0_Write(PC0_Write),
        .IR_Write(IR_Write),
        .Reg_Write(Reg_Write),
        .Mem_Write(Mem_Write),
        .rs2_imm_s(rs2_imm_s),
        .w_data_s(w_data_s),
        .PC_s(PC_s),
        .ALU_OP_o(ALU_OP_o)
    );

    reg_file32 reg_file32_inst (
    .Reg_Write(Reg_Write),
    .R_Addr_A(rs1),
    .R_Addr_B(rs2),
    .W_Addr(rd),
    .W_Data(F_),
    .R_Data_A(reg_a),
    .R_Data_B(reg_b),
    .clk_Regs(~clk),
    .rst_n(rst_n)
    );

    alu ALU (
        .ALU_OP(ALU_OP_o),
        .ALU_A(alu_a),
        .ALU_B(alu_b),
        .ZF(alu_fr[3]),
        .SF(alu_fr[2]),
        .OF(alu_fr[1]),
        .CF(alu_fr[0]),
        .ALU_F(alu_f)
    );

    reg_b32 A (
        .clk(~clk),
        .rst_n(rst_n),
        .d(reg_a),
        .q(alu_a)
    );

    reg_b32 B (
        .clk(~clk),
        .rst_n(rst_n),
        .d(reg_b),
        .q(Reg_B)
    );

    reg_b32 result_reg (
        .clk(~clk),
        .rst_n(rst_n),
        .d(alu_f),
        .q(F)
    );

    reg_b4 flag_reg (
        .clk(~clk),
        .rst_n(rst_n),
        .d(alu_fr),
        .q(FR)
    );

    mem_write mem_write_inst(
        .clk_dm(clk),
        .DM_Addr(F),
        .M_W_Data(Reg_B),
        .M_R_Data(M_R_Data),
        .Mem_Write(Mem_Write),
        .rst_n(rst_n)
    );

    mdr MDR(
        .rst_n(rst_n),
        .clk_dm_n(~clk),
        .MDR_i(M_R_Data),
        .MDR_o(MDR_o)
    );

endmodule