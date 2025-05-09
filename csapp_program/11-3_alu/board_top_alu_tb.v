`timescale 1ns / 1ps
module board_top_alu_tb();
    // 时钟和开关信号
    reg clk = 0;          // 系统时钟
    reg [1:4] swb;        // 功能按钮
    reg [1:32] sw;        // 数据开关
    
    // 输出信号
    wire [1:4] led;       // 状态指示灯 [ZF, SF, OF, CF]
    wire [7:0] seg;       // 七段数码管段选信号
    wire [2:0] which;     // 七段数码管位选信号
    wire enable;          // 使能信号
    wire F;
    board_top uut (
        .F(F),
        .clk(clk),
        .swb(swb),
        .sw(sw),
        .led(led),
        .seg(seg),
        .which(which),
        .enable(enable)
    );
    
    // 时钟生成
    always #0.01 clk = ~clk;
    
    // 测试过程
    initial begin
        // 初始化信号
        swb = 4'b0000;
        sw = 32'h00000000;
        
        // 复位释放
        #100;
        swb[4] = 1;      // 释放复位 (rst_n)
        #50;
        
        // 测试用例1: 加法操作 (ALU_OP = 0000)
        // 加载A=5
        sw = 32'h00000005;
        #10;
        swb[1] = 1;
        #10;
        swb[1] = 0;       // 释放clk_A信号
        
        // 加载B=3
        sw = 32'h00000003;  // 设置数据为操作数B
        #10;
        swb[2] = 1;       // 触发clk_B信号
        #10;
        swb[2] = 0;       // 释放clk_B信号
        
        // 执行加法并存储结果
        sw = 32'h00000000;  // 设置操作码为加法(0000)
        #10;
        swb[3] = 1;       // 触发clk_F信号执行运算
        #10;
        swb[3] = 0;       // 释放clk_F信号
        #50;              // 等待结果稳定并显示
   
        
        // 复位测试
        swb[4] = 0;       // 激活复位信号
        #20;
        swb[4] = 1;       // 释放复位信号
        #50;
        
        // 仿真完成
        #100;
        $finish;
    end
    
    // 监控关键信号
    initial begin
        $monitor("Time=%t, OP=%h, A=%h, B=%h, led=%b, seg=%h, which=%d,F=%h", 
                 $time, sw[1:4], sw, sw, led, seg, which, F);
    end
    
endmodule