// id2 次级译码器
module id2(
    opcode, funct3, funct7, 
    IS_R, IS_IMM, IS_LUI, IS_LW, IS_SW, IS_BEQ, IS_JAL, IS_JALR, ALU_OP
);

    input [6:0] opcode;
    input [2:0] funct3;
    input [6:0] funct7;
    output reg IS_R, IS_IMM, IS_LUI, IS_LW, IS_SW, IS_BEQ, IS_JAL, IS_JALR;
    output reg [3:0] ALU_OP;

    always @(*) begin
        // 默认值
        IS_R = 0;
        IS_IMM = 0;
        IS_LUI = 0;
        IS_LW = 0;
        IS_SW = 0;
        IS_BEQ = 0;
        IS_JAL = 0;
        IS_JALR = 0;
        ALU_OP = 4'b0000; // 默认ALU操作为加法

        case (opcode)
            7'b0110011: begin // R型指令
                IS_R = 1;
                ALU_OP = {funct7[5], funct3}; // R指令ALU操作码由funct7次高位和 funct3拼接形成
            end
            7'b0010011: begin // I型指令
                IS_IMM = 1;
                ALU_OP = (funct3 == 3'b101) ? {funct7[5], funct3} : {1'b0, funct3};  // 判断是否为位移指令
            end
            7'b0110111: begin // LUI指令
                IS_LUI = 1;
                ALU_OP = 4'b0001; // LUI操作码
            end
            7'b0000011: begin // LW指令
                IS_LW = 1;
                ALU_OP = 4'b0000;
            end
            7'b0100011: begin // SW指令
                IS_SW = 1;
                ALU_OP = 4'b0000;
            end
            7'b1100011: begin // B型指令
                IS_BEQ = 1;
                ALU_OP = 4'b1000;
            end
            7'b1100111: begin // JALR指令
                IS_JALR = 1;
                ALU_OP = 4'b0000;
            end
            7'b1101111: begin // JAL指令
                IS_JAL = 1;
                ALU_OP = 4'b0000;
            end
        endcase
    end

endmodule