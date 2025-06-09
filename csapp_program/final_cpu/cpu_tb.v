`timescale 1ns / 1ps

module cpu_tb();
    // 输入信号 - 模拟板级实验的按键
    reg clk;
    reg rst_n;
    
    // 测试控制变量
    integer step_count = 0;
    reg [31:0] prev_pc = 32'hFFFFFFFF;
    reg [31:0] prev_inst = 32'h00000000;
    
    // 实例化被测试的CPU模块
    cpu uut (
        .clk(clk),
        .rst_n(rst_n)
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
            #5;
            clk = 1;
            #5;
            
            // 显示当前状态
            display_cpu_state();
        end
    endtask
    
    // 任务：显示CPU当前状态
    task display_cpu_state();
        begin
            $display("--- CPU状态显示 ---");
            $display("PC = 0x%08h", uut.PC_o);
            $display("IR = 0x%08h (%s)", uut.inst, decode_simple_instruction(uut.inst));
            $display("控制单元状态: %s", get_state_name(uut.CU.ST));
            
            // 显示关键控制信号
            $display("控制信号: PC_Write=%b, IR_Write=%b, Reg_Write=%b, Mem_Write=%b", 
                     uut.PC_Write, uut.IR_Write, uut.Reg_Write, uut.Mem_Write);
            
            // 检查PC变化
            if (uut.PC_o != prev_pc) begin
                if (uut.PC_o == prev_pc + 4) begin
                    $display(">>> PC正确递增: 0x%08h -> 0x%08h (+4)", prev_pc, uut.PC_o);
                end else begin
                    $display(">>> PC跳转: 0x%08h -> 0x%08h", prev_pc, uut.PC_o);
                end
                prev_pc = uut.PC_o;
            end
            
            // 检查指令变化
            if (uut.inst != prev_inst) begin
                $display(">>> 新指令加载: 0x%08h", uut.inst);
                prev_inst = uut.inst;
            end
            
            // 显示寄存器操作
            if (uut.Reg_Write) begin
                $display(">>> 寄存器写入: R%0d <- 0x%08h", uut.rd, uut.F_);
            end
            
            // 显示内存操作
            if (uut.Mem_Write) begin
                $display(">>> 内存写入: [0x%08h] <- 0x%08h", uut.F, uut.Reg_B);
            end
            
            $display("");
        end
    endtask
    
    // 函数：获取状态名称
    function [127:0] get_state_name;
        input [3:0] state;
        begin
            case (state)
                4'b0000: get_state_name = "Idle";
                4'b0001: get_state_name = "S1_Fetch";
                4'b0010: get_state_name = "S2_Decode";
                4'b0011: get_state_name = "S3_R_Execute";
                4'b0100: get_state_name = "S4_WriteBack";
                4'b0101: get_state_name = "S5_I_Execute";
                4'b0110: get_state_name = "S6_LUI";
                4'b0111: get_state_name = "S7_Mem_Addr";
                4'b1000: get_state_name = "S8_Mem_Read";
                4'b1001: get_state_name = "S9_Mem_WB";
                4'b1010: get_state_name = "S10_Mem_Write";
                4'b1011: get_state_name = "S11_JAL";
                4'b1100: get_state_name = "S12_JALR";
                4'b1101: get_state_name = "S13_Branch";
                4'b1110: get_state_name = "S14_Branch_Decide";
                default: get_state_name = "UNKNOWN";
            endcase
        end
    endfunction
    
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