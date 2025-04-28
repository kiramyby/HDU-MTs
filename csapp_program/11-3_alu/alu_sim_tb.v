`timescale 1ns / 1ps

module alu_sim_tb;
    // 输入信号定义
    reg clk_A;
    reg clk_B;
    reg clk_F;
    reg rst_n;
    reg [3:0] ALU_OP;
    reg [31:0] ALU_A;
    reg [31:0] ALU_B;
    
    // 输出信号
    wire [31:0] ALU_F;
    wire [3:0] ALU_FR;
    wire ZF, CF, OF, SF;
    
    // 实例化alu模块进行直接测试
    alu alu_inst (
        .ALU_OP(ALU_OP),
        .ALU_A(ALU_A),
        .ALU_B(ALU_B),
        .ALU_F(ALU_F),
        .ZF(ZF),
        .CF(CF),
        .OF(OF),
        .SF(SF)
    );
    
    // 时钟生成
    always #5 clk_A = ~clk_A;
    always #6 clk_B = ~clk_B;
    always #7 clk_F = ~clk_F;
    
    // 波形输出
    initial begin
        $dumpfile("alu_sim.vcd");
        $dumpvars(0, alu_sim_tb);
    end
    
    // 格式化显示结果
    task display_result;
        input [4:0] op_type;
        input [31:0] a, b, result;
        input zf, cf, of, sf;
        
        begin
            case(op_type)
                0: $display("============= 测试: AND操作 =============");
                1: $display("============= 测试: OR操作 =============");
                2: $display("============= 测试: XOR操作 =============");
                3: $display("============= 测试: ADD操作 =============");
                4: $display("============= 测试: SUB操作 =============");
                5: $display("============= 测试: SLT操作 =============");
                6: $display("============= 测试: ADD溢出测试 =============");
                7: $display("============= 测试: 零标志测试 =============");
                default: $display("============= 未知操作 =============");
            endcase
            
            $display("操作数A = 0x%h (%d)", a, a);
            $display("操作数B = 0x%h (%d)", b, b);
            $display("结果    = 0x%h (%d)", result, result);
            $display("标志位状态:");
            $display("  ZF(零标志) = %b", zf);
            $display("  CF(进位标志) = %b", cf);
            $display("  OF(溢出标志) = %b", of);
            $display("  SF(符号标志) = %b", sf);
            $display("=====================================\n");
        end
    endtask
    
    // 初始化和测试过程
    initial begin
        // 初始化信号
        clk_A = 0;
        clk_B = 0;
        clk_F = 0;
        rst_n = 0;
        ALU_OP = 0;
        ALU_A = 0;
        ALU_B = 0;
        
        // 复位释放
        #20 rst_n = 1;
        
        // 测试不同的ALU操作
        // 测试1: AND操作 (7)
        #10;
        ALU_A = 32'h0000_0003;
        ALU_B = 32'h0000_0607;
        ALU_OP = 4'h7;      // AND操作
        #20;
        display_result(0, ALU_A, ALU_B, ALU_F, ZF, CF, OF, SF);
        
        // 测试2: OR操作 (6)
        #10;
        ALU_OP = 4'h6;      // OR操作
        #20;
        display_result(1, ALU_A, ALU_B, ALU_F, ZF, CF, OF, SF);
        
        // 测试3: XOR操作 (4)
        #10;
        ALU_OP = 4'h4;      // XOR操作
        #20;
        display_result(2, ALU_A, ALU_B, ALU_F, ZF, CF, OF, SF);
        
        // 测试4: ADD操作 (0)
        #10;
        ALU_OP = 4'h0;      // ADD操作
        #20;
        display_result(3, ALU_A, ALU_B, ALU_F, ZF, CF, OF, SF);
        
        // 测试5: SUB操作 (8)
        #10;
        ALU_OP = 4'h8;      // SUB操作
        #20;
        display_result(4, ALU_A, ALU_B, ALU_F, ZF, CF, OF, SF);
        
        // 测试6: SLT操作 (2)
        #10;
        ALU_OP = 4'h2;      // SLT操作
        #20;
        display_result(5, ALU_A, ALU_B, ALU_F, ZF, CF, OF, SF);
        
        // 测试7: 测试溢出情况 (ADD)
        #10;
        ALU_A = 32'h8000_0000;
        ALU_B = 32'h8000_0000;
        ALU_OP = 4'h0;      // ADD操作
        #20;
        display_result(6, ALU_A, ALU_B, ALU_F, ZF, CF, OF, SF);
        
        // 测试8: 测试零标志 (SUB相等的数)
        #10;
        ALU_A = 32'h7FFF_FFFF;
        ALU_B = 32'h7FFF_FFFF;
        ALU_OP = 4'h8;      // SUB操作
        #20;
        display_result(7, ALU_A, ALU_B, ALU_F, ZF, CF, OF, SF);
        
        // 结束仿真
        #10 $finish;
    end
endmodule