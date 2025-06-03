`timescale 1ns / 1ps

module cpu_tb();
    // 输入信号
    reg clk;
    reg rst_n;
    
    // 实例化被测试的CPU模块
    cpu uut (
        .clk(clk),
        .rst_n(rst_n)
    );
    
    // 时钟生成 - 50MHz时钟 (周期20ns)
    initial begin
        clk = 0;
        forever #10 clk = ~clk;
    end
    
    // 测试序列
    initial begin
        // 初始化波形记录
        $dumpfile("cpu_tb.vcd");
        $dumpvars(0, cpu_tb);
        
        // 显示测试开始信息
        $display("=== RISC-V CPU 测试开始 ===");
        $display("时间: %0t", $time);
        $display("测试程序: 数组求和程序");
        $display("预期功能: 从地址0x10读取3个数值(1,2,3)，求和后存储到地址0x30");
        
        // 初始化信号
        rst_n = 0;
        
        // 保持复位状态
        #100;
        $display("时间: %0t - 释放复位信号", $time);
        rst_n = 1;
        
        // 验证初始状态
        #50;
        $display("\n=== 初始状态验证 ===");
        $display("PC初始值: %h (应该为0)", uut.PC_o);
        $display("复位后第一条指令: %h", uut.inst_code);
        
        // 运行足够长的时间让程序完成
        // 基于分析，程序大约需要执行17条指令，每条指令需要多个时钟周期
        #10000;
        
        // 显示中间状态
        $display("\n=== CPU 中间状态 ===");
        $display("时间: %0t", $time);
        $display("PC值: %h", uut.PC_o);
        $display("当前指令: %h", uut.inst);
        $display("控制单元状态: PC_Write=%b, IR_Write=%b, Reg_Write=%b, Mem_Write=%b", 
                 uut.PC_Write, uut.IR_Write, uut.Reg_Write, uut.Mem_Write);
        
        // 继续运行
        #10000;
        
        // 显示最终状态
        $display("\n=== CPU 最终状态 ===");
        $display("时间: %0t", $time);
        $display("PC值: %h", uut.PC_o);
        $display("ALU结果: %h", uut.F);
        $display("标志位: %b", uut.FR);
        
        // 验证寄存器文件状态
        $display("\n=== 寄存器文件状态 ===");
        $display("寄存器x10(a0): %h (应为00000010)", uut.reg_file32_inst.reg_file[10]);
        $display("寄存器x11(a1): %h (应为00000003)", uut.reg_file32_inst.reg_file[11]);  
        $display("寄存器x12(a2): %h (应为00000030)", uut.reg_file32_inst.reg_file[12]);
        $display("寄存器x8(s0): %h (应为00000006)", uut.reg_file32_inst.reg_file[8]);
        $display("寄存器x5(t0): %h", uut.reg_file32_inst.reg_file[5]);
        $display("寄存器x6(t1): %h", uut.reg_file32_inst.reg_file[6]);
        $display("寄存器x7(t2): %h (应为00000006)", uut.reg_file32_inst.reg_file[7]);
        
        // 验证内存状态（通过内存模块访问）
        $display("\n=== 内存状态验证 ===");
        $display("源数据地址16(0x10): %h (应为00000001)", uut.mem_write_inst.Mem.reg_file[16]);
        $display("源数据地址17(0x11): %h (应为00000002)", uut.mem_write_inst.Mem.reg_file[17]);
        $display("源数据地址18(0x12): %h (应为00000003)", uut.mem_write_inst.Mem.reg_file[18]);
        $display("结果地址48(0x30): %h (应为00000006)", uut.mem_write_inst.Mem.reg_file[48]);
        
        // 程序验证
        if (uut.mem_write_inst.Mem.reg_file[48] == 32'h00000006) begin
            $display("\n✓ 测试通过：数组求和结果正确 (1+2+3=6)");
        end else begin
            $display("\n✗ 测试失败：数组求和结果错误");
            $display("  预期值: 00000006, 实际值: %h", uut.mem_write_inst.Mem.reg_file[48]);
        end
        
        $display("\n=== 测试完成 ===");
        $finish;
    end
    
    // 监控重要信号变化
    always @(posedge clk) begin
        if (rst_n) begin
            // 监控PC变化
            if (uut.PC_Write) begin
                $display("时间: %0t - PC更新: %h -> %h", $time, uut.PC_o, uut.PC_i);
            end
            
            // 监控指令获取
            if (uut.IR_Write) begin
                $display("时间: %0t - 指令获取: %h (PC=%h)", $time, uut.inst_code, uut.PC_o);
            end
            
            // 监控寄存器写入
            if (uut.Reg_Write) begin
                $display("时间: %0t - 寄存器写入: R%0d <- %h", $time, uut.rd, uut.F_);
            end
            
            // 监控内存写入
            if (uut.Mem_Write) begin
                $display("时间: %0t - 内存写入: Addr=%h, Data=%h", $time, uut.F, uut.Reg_B);
            end
            
            // 监控ALU操作
            if (uut.CU.ST == 4'b0011 || uut.CU.ST == 4'b0101 || uut.CU.ST == 4'b1101) begin // S3, S5, S13 states
                $display("时间: %0t - ALU操作: OP=%b, A=%h, B=%h -> F=%h", 
                         $time, uut.ALU_OP_o, uut.alu_a, uut.alu_b, uut.alu_f);
            end
        end
    end
    
    // 错误检测
    initial begin
        // 检查是否有未定义的信号
        #50;
        if (rst_n === 1'bx) begin
            $display("错误: rst_n信号未定义");
            $finish;
        end
    end
    
    // 超时保护
    initial begin
        #50000; // 50微秒超时
        $display("警告: 测试超时");
        $finish;
    end

endmodule