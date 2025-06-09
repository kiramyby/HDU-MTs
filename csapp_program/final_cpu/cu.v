// CU 控制单元
module CU (
    IS_R, IS_IMM, IS_LUI, IS_LW, IS_SW, IS_BEQ, IS_JALR, IS_JAL, clk, rst_n, ALU_OP,
    ZF_rs,
    PC_Write, PC0_Write, IR_Write, Reg_Write, Mem_Write, rs2_imm_s, w_data_s, PC_s, ALU_OP_o
);
    input IS_R, IS_IMM, IS_LUI, IS_LW, IS_SW, IS_BEQ, IS_JALR, IS_JAL, ZF_rs;
    input clk, rst_n;
    input [3:0] ALU_OP;
    output reg PC_Write, PC0_Write, IR_Write, Reg_Write, Mem_Write;
    output reg rs2_imm_s;
    output reg [1:0] w_data_s, PC_s;
    output reg [3:0] ALU_OP_o;
    reg [3:0] ST, ST_nx; // 状态寄存器和下一个状态寄存器
    
    // 状态参数定义
    parameter Idle = 4'b0000;
    parameter S1   = 4'b0001;
    parameter S2   = 4'b0010;
    parameter S3   = 4'b0011;
    parameter S4   = 4'b0100;
    parameter S5   = 4'b0101;
    parameter S6   = 4'b0110;
    parameter S7   = 4'b0111;
    parameter S8   = 4'b1000;
    parameter S9   = 4'b1001;
    parameter S10  = 4'b1010;
    parameter S11  = 4'b1011;
    parameter S12  = 4'b1100;
    parameter S13  = 4'b1101;
    parameter S14  = 4'b1110;

    // 状态寄存器更新 - 时序逻辑
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ST <= Idle;
        end else begin
            ST <= ST_nx;
        end
    end
    // 状态转移逻辑 - 组合逻辑  
    always @(*) begin
        ST_nx = Idle; // 默认状态为Idle
        case (ST)
            Idle: ST_nx = S1;
            S1: begin
                if (IS_LUI) ST_nx = S6;
                else if (IS_JAL) ST_nx = S11;
                else ST_nx = S2;
            end
            S2: begin
                if (IS_R) ST_nx = S3;
                else if (IS_IMM) ST_nx = S5;
                else if (IS_BEQ) ST_nx = S13;
                else ST_nx = S7;
            end
            
            S3: begin
                ST_nx = S4; // R型指令
            end

            S4: begin
                ST_nx = S1;
            end

            S5: begin
                ST_nx = S4; // I型指令
            end

            S6: begin
                ST_nx = S1; // LUI指令
            end

            S7: begin
                if (IS_LW) ST_nx = S8;
                else if (IS_SW) ST_nx = S10;
                else ST_nx = S12; // JALR指令
            end

            S8: begin
                ST_nx = S9;
            end

            S9: begin
                ST_nx = S1;
            end
            
            S10: begin
                ST_nx = S1; // SW指令
            end

            S11: begin
                ST_nx = S1;
            end

            S12: begin
                ST_nx = S1; // JALR指令
            end

            S13: begin
                ST_nx = S14;
            end

            S14: begin
                ST_nx = S1;
            end

            default: ST_nx = Idle;
        endcase
    end

    // 控制信号输出逻辑
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            PC_Write <= 0;
            PC0_Write <= 0;
            IR_Write <= 0;
            Reg_Write <= 0;
            Mem_Write <= 0;
            rs2_imm_s <= 0;
            w_data_s <= 2'b00;
            PC_s <= 2'b00;
            ALU_OP_o <= 4'b0000;
        end
        else begin
            case (ST_nx)
                S1: begin
                    PC_Write <= 1;
                    PC0_Write <= 1;
                    IR_Write <= 1;
                    Reg_Write <= 0;
                    Mem_Write <= 0;
                    PC_s <= 2'b00;
                end

                S2: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 0;
                    Mem_Write <= 0;
                end

                S3: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 0;
                    Mem_Write <= 0;
                    ALU_OP_o <= ALU_OP;
                    rs2_imm_s <= 0;
                end

                S4: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 1;
                    Mem_Write <= 0;
                    w_data_s <= 2'b00;
                end

                S5: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 0;
                    Mem_Write <= 0;
                    ALU_OP_o <= ALU_OP;
                    rs2_imm_s <= 1;
                end

                S6: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 1;
                    Mem_Write <= 0;
                    w_data_s <= 2'b01;
                end

                S7: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 0;
                    Mem_Write <= 0;
                    ALU_OP_o <= 4'b0000;
                    rs2_imm_s <= 1;
                end

                S8: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 0;
                    Mem_Write <= 0;
                end

                S9: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 1;
                    Mem_Write <= 0;
                    w_data_s <= 2'b10;
                end

                S10: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 0;
                    Mem_Write <= 1;
                end

                S11: begin
                    PC_Write <= 1;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 1;
                    Mem_Write <= 0;
                    w_data_s <= 2'b11;
                    PC_s <= 2'b01;
                end

                S12: begin
                    PC_Write <= 1;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 1;
                    Mem_Write <= 0;
                    w_data_s <= 2'b11;
                    PC_s <= 2'b10;
                end

                S13: begin
                    PC_Write <= 0;
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 0;
                    Mem_Write <= 0;
                    ALU_OP_o <= 4'b1000;
                    rs2_imm_s <= 0;
                end

                S14: begin
                    PC_Write <= ZF_rs; 
                    PC0_Write <= 0;
                    IR_Write <= 0;
                    Reg_Write <= 0;
                    Mem_Write <= 0;
                    PC_s <= 2'b01;
                end
            endcase
        end
    end

endmodule