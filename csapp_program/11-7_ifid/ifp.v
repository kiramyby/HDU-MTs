// 指令字段解析器 inst field parser
module ifp (
    inst, rst_n, clk,
    rs1, rs2, rd, opcode, funct3, funct7
);
    input [31:0] inst; 
    input rst_n; 
    input clk;
    output reg [4:0] rs1, rs2, rd;
    output reg [6:0] opcode;
    output reg [2:0] funct3;
    output reg [6:0] funct7;
    output reg [31:0] imm32;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            rs1 <= 5'b0;
            rs2 <= 5'b0;
            rd <= 5'b0;
            opcode <= 7'b0;
            funct3 <= 3'b0;
            funct7 <= 7'b0;
            imm32 <= 32'b0;
        end else begin
            rs1 <= inst[19:15];
            rs2 <= inst[24:20];
            rd <= inst[11:7];
            opcode <= inst[6:0];
            funct3 <= inst[14:12];
            funct7 <= inst[31:25];
        end
    end
)