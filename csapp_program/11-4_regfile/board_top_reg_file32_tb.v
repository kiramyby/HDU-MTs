`timescale 1ns / 1ps

module board_top_reg_file32_tb();
    // Inputs
    reg clk;
    reg [1:4] swb;
    reg [1:32] sw;
    
    // Outputs
    wire [1:4] led;
    wire [7:0] seg;
    wire [2:0] which;
    wire enable;
    
    // Internal signals to monitor (hierarchical references)
    wire [31:0] reg_a;
    wire [31:0] reg_b;
    wire [31:0] alu_a;
    wire [31:0] alu_b;
    wire [31:0] alu_f;
    wire [31:0] F;
    wire [31:0] F_;
    
    // Instantiate the Unit Under Test (UUT)
    board_top__reg_file32 uut (
        .clk(clk),
        .swb(swb),
        .sw(sw),
        .led(led),
        .seg(seg),
        .which(which),
        .enable(enable)
    );
    
    // Connecting to internal signals for monitoring
    assign reg_a = uut.reg_file32_top_inst.reg_a;
    assign reg_b = uut.reg_file32_top_inst.reg_b;
    assign alu_a = uut.reg_file32_top_inst.alu_a;
    assign alu_b = uut.reg_file32_top_inst.alu_b;
    assign alu_f = uut.reg_file32_top_inst.alu_f;
    assign F = uut.reg_file32_top_inst.F;
    assign F_ = uut.reg_file32_top_inst.F_;
    
    // Clock generation
    initial begin
        clk = 0;
        forever #0.01 clk = ~clk;
    end
    
    // Test sequence
    initial begin
        // Initialize Inputs
        swb = 4'b0000;
        sw = 32'h00000000;
        
        // Reset the system
        sw[27] = 0;  // rst_n = 0
        #20;
        sw[27] = 1;  // rst_n = 1
        
        // Test case 1: Write a value to register 1
        #10;
        sw[1:5] = 5'd1;     // W_Addr = 5'd1 (Register 1)
        sw[16:19] = 4'b1010; // cus = 4'b1010 (Custom value)
        swb[4] = 1;         // cus_enable = 1
        sw[28] = 1;         // Reg_Write = 1
        sw[6:10] = 5'd1;    // R_Addr_A = 5'd1
        sw[11:15] = 5'd2;   // R_Addr_B = 5'd2
        #10;
        swb[3] = 1;         // clk_WB = 1
        #10;
        swb[3] = 0;         // clk_WB = 0
        
        // Execute read operation
        #10;
        swb[1] = 1;         // clk_RR = 1
        #10;
        swb[1] = 0;         // clk_RR = 0
        #10;
        
        // Test case 2: Write to register 2
        sw[1:5] = 5'd2;     // W_Addr = 5'd2
        #10;
        swb[3] = 1;         // clk_WB = 1
        #10;
        swb[3] = 0;         // clk_WB = 0
        #10;
        swb[1] = 1;         // clk_RR = 1
        #10;
        swb[1] = 0;         // clk_RR = 0
        
        // Perform ALU operation
        #10;
        sw[29:32] = 4'b0000; // ALU_OP = 0 (ADD)
        swb[2] = 1;         // clk_F = 1
        #10;
        swb[2] = 0;         // clk_F = 0
        swb[4] = 0;         // cus_enable = 0
        
        // Write result back
        #10;
        swb[3] = 1;         // clk_WB = 1
        #10;
        swb[3] = 0;         // clk_WB = 0
        sw[28] = 0;         // Reg_Write = 0
        
        // Wait to observe results
        #1000;
        $finish;
    end
    
    // Monitor for internal signals
    initial begin
        $monitor("Time=%0t | R_Addr_A=%0d, R_Addr_B=%0d, W_Addr=%0d | reg_a=%0h, reg_b=%0h, alu_a=%0h, alu_b=%0h | ALU_OP=%0b, alu_f=%0h | F=%0h, led=%0b",
                 $time, sw[6:10], sw[11:15], sw[1:5], reg_a, reg_b, alu_a, alu_b, sw[29:32], alu_f, F, led);
    end
    
    // Display register file values at the end
    initial begin
        #290;
        $display("Final register values:");
        $display("Reg[1] = %0h", uut.reg_file32_top_inst.reg_file32_inst.reg_file[1]);
        $display("Reg[2] = %0h", uut.reg_file32_top_inst.reg_file32_inst.reg_file[2]);
        $display("Reg[3] = %0h", uut.reg_file32_top_inst.reg_file32_inst.reg_file[3]);
        $display("Reg[4] = %0h", uut.reg_file32_top_inst.reg_file32_inst.reg_file[4]);
    end
    
endmodule
