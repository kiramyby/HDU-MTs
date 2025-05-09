`timescale 1ns / 1ps
// ALU多功能测试模块仿真

module multi_alu_tb();
    // 时钟和复位信号
    reg clk = 0;      
    reg clk_A = 0;
    reg clk_B = 0;
    reg clk_F = 0;
    reg rst_n = 0;        
    reg [31:0] sw;
  
    wire [3:0] FR; 
    wire [7:0] seg; 
    wire [2:0] which;
    
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
    
    // 时钟生成
    always #0.01 clk = ~clk; 
    
    // 测试过程
    initial begin
        // 初始化信号
        sw = 32'h0;
        rst_n = 0;
        #100;
        rst_n = 1;
        // 加载A=5
        sw = 32'h00000005;  // 设置低位为操作数
        #10 clk_A = 1;
        #10 clk_A = 0;
        
        // 加载B=3
        sw = 32'h00000003;
        #10 clk_B = 1;
        #10 clk_B = 0;

        // 执行加法并存储结果
        sw = 32'h00000000; 
        #10 clk_F = 1;
        #10 clk_F = 0;
        #600;  // 等待数码管结果显示
        
        // 复位释放
        rst_n = 0;
        #10 rst_n = 1;

        // 加载A=5
        sw = 32'h00000001;  // 设置低位为操作数
        #10 clk_A = 1;
        #10 clk_A = 0;
        // 加载B=3
        sw = 32'h00000000;
        #10 clk_B = 1;
        #10 clk_B = 0;
        // 执行异或并存储结果
        sw = 32'h00000004; 
        #10 clk_F = 1;
        #10 clk_F = 0;
        #500;  // 等待数码管结果显示
                  
        $finish;
    end
    
endmodule