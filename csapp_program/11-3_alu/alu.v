module alu (
    ALU_OP, ALU_A, ALU_B, ALU_F,
    ZF, CF, OF, SF
);

input  wire [3:0]  ALU_OP;      
input  wire [31:0] ALU_A;      
input  wire [31:0] ALU_B;       
output reg  [31:0] ALU_F;
output wire        ZF;           // 零标志
output wire        CF;           // 进位标志
output wire        OF;           // 溢出标志
output wire        SF;           // 符号标志

reg C32 = 1'b0;

always @(ALU_OP, ALU_A, ALU_B)
begin
    case (ALU_OP)
        4'b0000: begin  // 加法 add
            {C32, ALU_F} = ALU_A + ALU_B;
        end
        4'b0001: begin  // 左移 sll
            {C32, ALU_F} = {1'b0, ALU_A << ALU_B[4:0]};
        end
        4'b0010: ALU_F = ($signed(ALU_A) < $signed(ALU_B)) ? 32'd1 : 32'd0;  // 有符号数比较小于置数 slt
        4'b0011: ALU_F = (ALU_A < ALU_B) ? 32'd1 : 32'd0;  // 无符号数比较小于置数 sltu
        4'b0100: ALU_F = ALU_A ^ ALU_B;           // 异或 xor
        4'b0101: begin  // 逻辑右移 srl
            {C32, ALU_F} = {1'b0, ALU_A >> ALU_B[4:0]};
        end
        4'b0110: ALU_F = ALU_A | ALU_B;           // 按位或 or
        4'b0111: ALU_F = ALU_A & ALU_B;           // 按位与 and
        4'b1000: begin  // 减法 sub
            {C32, ALU_F} = ALU_A - ALU_B;
        end
        4'b1101: begin  // 算术右移 sra
            {C32, ALU_F} = {ALU_A[31], $signed(ALU_A) >>> ALU_B[4:0]};
        end
        default: {C32, ALU_F} = 33'b0;
    endcase
end

// FlagU
assign ZF = (ALU_F == 32'd0);
assign SF = ALU_F[31];
assign CF = (ALU_OP == 4'b0000) ? C32 :
            (ALU_OP == 4'b1000) ? ~C32 : 1'b0;
assign OF = ALU_A[31] ^ ALU_B[31] ^ C32 ^ ALU_F[31];

endmodule