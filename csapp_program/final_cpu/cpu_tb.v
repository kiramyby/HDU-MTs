`timescale 1ns / 1ps

module cpu_tb();
    // 输入信号 - 模拟板级实验的按键
    reg clk;
    reg rst_n;
    
    // 输出信号 - CPU暴露的观察端口
    wire [31:0] PC_out;
    wire [31:0] IR_out;
    wire [31:0] MDR_out;
    wire [31:0] W_Data_out;
    
    // 测试控制变量
    integer step_count = 0;
    reg [31:0] prev_pc = 32'hFFFFFFFF;
    reg [31:0] prev_inst = 32'h00000000;
    
    // 实例化被测试的CPU模块
    cpu uut (
        .clk(clk),
        .rst_n(rst_n),
        .PC_out(PC_out),
        .IR_out(IR_out),
        .MDR_out(MDR_out),
        .W_Data_out(W_Data_out)
    );
    
    // 初始化
    initial begin
        clk = 0;
        rst_n = 1;
        
        // 初始化波形记录
        $dumpfile("cpu_tb.vcd");
        $dumpvars(0, cpu_tb);
        
        $display("=========================================");
        $display("===   RISC-V CPU 板级实验模拟测试   ===");
        $display("=========================================");
        $display("说明：模拟板级实验中的手动控制");
        $display("- rst_n按键：复位PC和IR到0");
        $display("- clk按键：手动步进时钟，执行一步");
        $display("观察端口：");
        $display("- PC_out：程序计数器值");
        $display("- IR_out：指令寄存器值");
        $display("- MDR_out：内存数据寄存器值");
        $display("- W_Data_out：写入寄存器堆的数据");
        $display("=========================================");
        
        // 等待一段时间后开始测试
        #50;
        
        // 步骤1：按下复位按键
        $display("\n【步骤1】按下rst_n按键 - 复位CPU");
        press_reset();
        
        // 等待复位完成
        #20;
        display_cpu_state();
        
        // 步骤2：释放复位，开始单步执行
        $display("\n【步骤2】释放rst_n，开始单步时钟执行");
        rst_n = 1;
        #10;
        
        // 单步执行多个时钟周期
        repeat(50) begin
            press_clock();
            #10;
        end
        
        $display("\n========== 测试完成 ==========");
        $finish;
    end
    
    // 任务：模拟按下复位按键
    task press_reset();
        begin
            $display(">>> 按下 rst_n 按键");
            rst_n = 0;
            #10;
            step_count = 0;
            prev_pc = 32'hFFFFFFFF;
            prev_inst = 32'h00000000;
        end
    endtask
    
    // 任务：模拟按下时钟按键
    task press_clock();
        begin
            step_count = step_count + 1;
            $display("\n>>> 按下 clk 按键 [第%0d步]", step_count);
            
            // 产生一个时钟上升沿
            clk = 0;
            #2;
            clk = 1;
            #2;
            
            // 显示当前状态
            display_cpu_state();
        end
    endtask
    
    // 任务：显示CPU当前状态
    task display_cpu_state();
        begin
            $display("--- CPU状态显示 ---");
            $display("PC = 0x%08h", PC_out);
            $display("IR = 0x%08h (%s)", IR_out, decode_simple_instruction(IR_out));
            $display("MDR = 0x%08h", MDR_out);
            $display("W_Data = 0x%08h", W_Data_out);
            
            // 检查PC变化
            if (PC_out != prev_pc) begin
                if (PC_out == prev_pc + 4) begin
                    $display(">>> PC正确递增: 0x%08h -> 0x%08h (+4)", prev_pc, PC_out);
                end else begin
                    $display(">>> PC跳转: 0x%08h -> 0x%08h", prev_pc, PC_out);
                end
                prev_pc = PC_out;
            end
            
            // 检查指令变化
            if (IR_out != prev_inst) begin
                $display(">>> 新指令加载: 0x%08h", IR_out);
                prev_inst = IR_out;
            end
            
            // 显示写入寄存器的数据（当有写入时）
            if (W_Data_out != 32'h00000000) begin
                $display(">>> 寄存器写入数据: 0x%08h", W_Data_out);
            end
            
            // 显示内存读取的数据（当有数据时）
            if (MDR_out != 32'h00000000) begin
                $display(">>> 内存读取数据: 0x%08h", MDR_out);
            end
            
            $display("");
        end
    endtask
    
    // 函数：简单指令解码
    function [255:0] decode_simple_instruction;
        input [31:0] inst;
        reg [6:0] opcode;
        reg [2:0] funct3;
        begin
            opcode = inst[6:0];
            funct3 = inst[14:12];
            
            case (opcode)
                7'b0110011: decode_simple_instruction = "R-type";
                7'b0010011: begin
                    case (funct3)
                        3'b000: decode_simple_instruction = "ADDI";
                        3'b110: decode_simple_instruction = "ORI";
                        3'b100: decode_simple_instruction = "XORI";
                        default: decode_simple_instruction = "I-type";
                    endcase
                end
                7'b0110111: decode_simple_instruction = "LUI";
                7'b0000011: decode_simple_instruction = "LW";
                7'b0100011: decode_simple_instruction = "SW";
                7'b1100011: decode_simple_instruction = "BEQ";
                7'b1101111: decode_simple_instruction = "JAL";
                7'b1100111: decode_simple_instruction = "JALR";
                default: decode_simple_instruction = "UNKNOWN";
            endcase
        end
    endfunction

endmodule