module alu (
    input  wire [3:0]  ALU_OP,      
    input  wire [31:0] ALU_A,       
    input  wire [31:0] ALU_B,       
    output reg  [31:0] ALU_F,
    output wire        ZF,           // Zero Flag
    output wire        CF,           // Carry Flag
    output wire        OF,           // Overflow Flag
    output wire        SF            // Sign Flag
);

reg C32 = 1'b0;

always @(ALU_OP, ALU_A, ALU_B)
begin
    case (ALU_OP)
        4'b0000: begin  // add
            {C32, ALU_F} = ALU_A + ALU_B;
        end
        4'b0001: begin  // sll
            {C32, ALU_F} = {1'b0, ALU_A << ALU_B[4:0]};
        end
        4'b0010: ALU_F = ($signed(ALU_A) < $signed(ALU_B)) ? 32'd1 : 32'd0;  // slt
        4'b0011: ALU_F = (ALU_A < ALU_B) ? 32'd1 : 32'd0;  // sltu
        4'b0100: ALU_F = ALU_A ^ ALU_B;           // xor
        4'b0101: begin  // srl
            {C32, ALU_F} = {1'b0, ALU_A >> ALU_B[4:0]};
        end
        4'b0110: ALU_F = ALU_A | ALU_B;           // or
        4'b0111: ALU_F = ALU_A & ALU_B;           // and
        4'b1000: begin  // sub
            {C32, ALU_F} = ALU_A - ALU_B;
        end
        4'b1101: begin  // sra
            {C32, ALU_F} = {ALU_A[31], $signed(ALU_A) >>> ALU_B[4:0]};
        end
        default: {C32, ALU_F} = 33'b0;
    endcase
end

// FlagU
assign ZF = (ALU_F == 32'd0);
assign SF = ALU_F[31];
assign CF = ((ALU_OP == 4'b0000) || (ALU_OP == 4'b1000) || 
            (ALU_OP == 4'b0001) || (ALU_OP == 4'b0101) || (ALU_OP == 4'b1101)) ? C32 : !C32;
assign OF = ALU_A[31] ^ ALU_B[31] ^ C32 ^ ALU_F[31];

endmodule