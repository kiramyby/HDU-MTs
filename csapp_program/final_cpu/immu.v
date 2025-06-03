// 定义RISC-V指令格式类型，后续生成立即数
`define OP_BRANCH 7'b1100011  // B型指令
`define OP_IMM    7'b0010011  // I型指令
`define OP_JAL    7'b1101111  // J型指令
`define OP_JALR   7'b1100111  // JALR型指令
`define OP_LW     7'b0000011  // Load型指令
`define OP_SW     7'b0100011  // Store型指令
`define OP_LUI    7'b0110111  // LUI指令

module immu (
    inst, imm32
);
    input [31:0] inst;
    output reg [31:0] imm32;
    reg [2:0] inst_Type; // 指令格式类型
    wire [6:0] opcode = inst[6:0]; // 指令操作码

    always @(*) begin
        case (opcode)
            `OP_BRANCH: begin
                inst_Type = 3'b011; // B型指令格式编码
                // B型指令立即数计算
                imm32 = {{19{inst[31]}}, inst[7], inst[30:25], inst[11:8], 1'b0};
            end
            `OP_IMM: begin
                inst_Type = 3'b100; // I型指令格式编码
                // I型指令立即数计算
                imm32 = {{20{inst[31]}}, inst[31:20]};
            end
            `OP_JAL: begin
                inst_Type = 3'b101; // J型指令格式编码
                // J型指令立即数计算
                imm32 = {{12{inst[31]}}, inst[19:12], inst[20], inst[30:21], 1'b0};
            end
            `OP_JALR: begin
                inst_Type = 3'b110; // JALR型指令格式编码
                // JALR型指令立即数计算
                imm32 = {{20{inst[31]}}, inst[31:20]};
            end
            `OP_LW: begin
                inst_Type = 3'b111; // Load型指令格式编码
                // Load型指令立即数计算
                imm32 = {{20{inst[31]}}, inst[31:20]};
            end
            `OP_SW: begin
                inst_Type = 3'b111; // Store型指令格式编码
                // Store型指令立即数计算
                imm32 = {{20{inst[31]}}, inst[31:25], inst[11:7]};
            end
            `OP_LUI: begin
                inst_Type = 3'b001; // LUI型指令格式编码
                // LUI型指令立即数计算
                imm32 = {inst[31:12], 12'b0};
            end
            default: begin
                // 未知指令，设置为0
                imm32 = 32'b0;
                inst_Type = 3'b000;
            end
        endcase
    end

endmodule
