`timescale 1ns / 1ps
// ALU多功能测试模块仿真

module multi_alu_tb();
    // 时钟和复位信号
    reg clk = 0;        // 初始化时钟
    reg clk_A = 0;
    reg clk_B = 0;
    reg clk_F = 0;
    reg rst_n = 0;      // 初始为复位状态
    
    // 输入信号
    reg [31:0] sw;
    
    // 输出信号
    wire [3:0] FR;      // 标志位 [ZF, SF, OF, CF]
    wire [7:0] seg;     // 七段数码管段选信号
    wire [2:0] which;   // 七段数码管位选信号
    
    // 实例化被测模块
    multi_alu uut (
        .clk(clk),
        .clk_A(clk_A),
        .clk_B(clk_B),
        .clk_F(clk_F),
        .rst_n(rst_n),
        .sw(sw),
        .FR(FR),
        .seg(seg),
        .which(which)
    );
    
    // 时钟生成 - 使用与display_test.v类似的简洁方式
    always #10 clk = ~clk;  // 周期为20ns，相当于50MHz
    
    // 测试过程
    initial begin
        // 初始化信号
        sw = 32'h0;
        
        // 复位释放
        #100;
        rst_n = 1;
        #50;
        
        // 测试用例1: 加法操作 (ALU_OP = 0000)
        // 加载A=5
        sw = 32'h00000005;  // 设置低位为操作数
        #10 clk_A = 1;
        #10 clk_A = 0;
        
        // 加载B=3
        sw = 32'h00000003;
        #10 clk_B = 1;
        #10 clk_B = 0;
        
        // 执行加法并存储结果
        sw = 32'h00000000;  // 设置操作码为加法(0000)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;  // 等待结果稳定
        
        // 测试用例2: 减法操作 (ALU_OP = 1000)
        // A保持为5，更改B为2
        sw = 32'h00000002;
        #10 clk_B = 1;
        #10 clk_B = 0;
        
        // 执行减法
        sw = 32'h80000000;  // 设置操作码为减法(1000)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 测试用例3: 逻辑与操作 (ALU_OP = 0111)
        // A=0xF0F0, B=0xFF00
        sw = 32'h0000F0F0;
        #10 clk_A = 1;
        #10 clk_A = 0;
        
        sw = 32'h0000FF00;
        #10 clk_B = 1;
        #10 clk_B = 0;
        
        // 执行逻辑与
        sw = 32'h70000000;  // 设置操作码为逻辑与(0111)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 测试用例4: 逻辑或操作 (ALU_OP = 0110)
        // A和B保持不变
        
        // 执行逻辑或
        sw = 32'h60000000;  // 设置操作码为逻辑或(0110)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 测试用例5: 异或操作 (ALU_OP = 0100)
        // 执行异或
        sw = 32'h40000000;  // 设置操作码为异或(0100)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 测试用例6: 左移操作 (ALU_OP = 0001)
        // A=0x1, B=4 (左移4位)
        sw = 32'h00000001;
        #10 clk_A = 1;
        #10 clk_A = 0;
        
        sw = 32'h00000004;
        #10 clk_B = 1;
        #10 clk_B = 0;
        
        // 执行左移
        sw = 32'h10000000;  // 设置操作码为左移(0001)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 测试用例7: 逻辑右移 (ALU_OP = 0101)
        // A=0x80000000, B=4 (右移4位)
        sw = 32'h80000000;
        #10 clk_A = 1;
        #10 clk_A = 0;
        
        sw = 32'h00000004;
        #10 clk_B = 1;
        #10 clk_B = 0;
        
        // 执行逻辑右移
        sw = 32'h50000000;  // 设置操作码为逻辑右移(0101)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 测试用例8: 算术右移 (ALU_OP = 1101)
        // A保持为0x80000000, B=4
        
        // 执行算术右移
        sw = 32'hD0000000;  // 设置操作码为算术右移(1101)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 测试用例9: 有符号比较 SLT (ALU_OP = 0010)
        // A=-5, B=3
        sw = 32'hFFFFFFFB;  // -5的二进制表示
        #10 clk_A = 1;
        #10 clk_A = 0;
        
        sw = 32'h00000003;
        #10 clk_B = 1;
        #10 clk_B = 0;
        
        // 执行有符号比较
        sw = 32'h20000000;  // 设置操作码为SLT(0010)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 测试用例10: 无符号比较 SLTU (ALU_OP = 0011)
        // 执行无符号比较
        sw = 32'h30000000;  // 设置操作码为SLTU(0011)
        #10 clk_F = 1;
        #10 clk_F = 0;
        #50;
        
        // 仿真完成
        #100;
        $finish;
    end
    
    // 监控关键信号 - 增加更详细的信息输出
    initial begin
        $monitor("Time=%t, OP=%h, A=%h, B=%h, F=%h, ZF=%b, SF=%b, OF=%b, CF=%b, seg=%b, which=%b", 
                 $time, uut.alu_op, uut.alu_a, uut.alu_b, uut.F, 
                 FR[3], FR[2], FR[1], FR[0], seg, which);
    end
    
endmodule