module reg_file32 ( 
    Reg_Write, R_Addr_A, R_Addr_B, W_Addr, W_Data, 
    R_Data_A, R_Data_B, clk_Regs, rst_n
);
    input [31:0] W_Data;
    input [4:0] R_Addr_A, R_Addr_B, W_Addr;
    input Reg_Write, clk_Regs, rst_n;

    output reg [31:0] R_Data_A, R_Data_B;

    reg [31:0] reg_file[0:31];
    integer  i;

    always @(posedge clk_Regs or negedge rst_n) begin
        if (!rst_n) begin
            for (i = 0; i < 32; i = i + 1) begin
                reg_file[i] = 32'b0;
            end
        end else if (Reg_Write && W_Addr != 5'b00000) begin
            reg_file[W_Addr] = W_Data;
        end
    end
    
    always @(*) begin
        R_Data_A = (R_Addr_A == 5'b00000) ? 32'b0 : reg_file[R_Addr_A]; 
        R_Data_B = (R_Addr_B == 5'b00000) ? 32'b0 : reg_file[R_Addr_B];
    end
    
endmodule