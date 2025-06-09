// id1 初级译码器
module id1 (
    inst, rs1, rs2, rd, opcode, funct3, funct7, imm32
);
    input [31:0] inst;
    output reg [4:0] rs1, rs2, rd;
    output reg [6:0] opcode;
    output reg [2:0] funct3;
    output reg [6:0] funct7;
    output [31:0] imm32;

    always @(*) begin
        rs1 = inst[19:15];
        rs2 = inst[24:20];
        rd = inst[11:7];
        opcode = inst[6:0];
        funct3 = inst[14:12];
        funct7 = inst[31:25];
    end

    immu ImmU (
        .inst(inst),
        .imm32(imm32)
    );

endmodule
