`timescale 1ns/1ps

module multi_alu_tb;

    // Inputs
    reg clk;
    reg clk_A;
    reg clk_B;
    reg clk_F;
    reg rst_n;
    reg [31:0] sw;
    
    // Outputs
    wire [3:0] FR;
    wire [7:0] SEG;
    wire [2:0] WHICH;
    
    // Instantiate the multi_alu
    multi_alu uut (
        .clk(clk),
        .clk_A(clk_A),
        .clk_B(clk_B),
        .clk_F(clk_F),
        .rst_n(rst_n),
        .sw(sw),
        .FR(FR),
        .SEG(SEG),
        .WHICH(WHICH)
    );
    
    // Clock generation
    initial begin
        clk = 0;
        forever #5 clk = ~clk; // 100MHz clock
    end
    
    // Test procedure
    initial begin
        // Initialize inputs
        clk_A = 0;
        clk_B = 0;
        clk_F = 0;
        rst_n = 0;
        sw = 0;
        
        // Apply reset
        #20;
        rst_n = 1;
        #10;
        
        // Test Case 1: Addition (A + B)
        // Load A with 10
        sw = 32'h0000000A;
        #5 clk_A = 1;
        #5 clk_A = 0;
        
        // Load B with 20
        sw = 32'h00000014;
        #5 clk_B = 1;
        #5 clk_B = 0;
        
        // Set operation to ADD (0000)
        sw = 32'h00000000;
        #5 clk_F = 1;
        #5 clk_F = 0;
        
        // Result should be 30 (0x1E) and ZF=0, SF=0, OF=0, CF=0
        #10;
        $display("Test Case 1 - Addition: A=%h, B=%h, F=%h, FR=%b", 
                 uut.alu_a, uut.alu_b, uut.F, FR);
                 
        // Test Case 2: Subtraction (A - B)
        // Load A with 30
        sw = 32'h0000001E;
        #5 clk_A = 1;
        #5 clk_A = 0;
        
        // Load B with 10
        sw = 32'h0000000A;
        #5 clk_B = 1;
        #5 clk_B = 0;
        
        // Set operation to SUB (1000)
        sw = 32'h80000000;
        #5 clk_F = 1;
        #5 clk_F = 0;
        
        // Result should be 20 (0x14) and ZF=0, SF=0, OF=0, CF=0
        #10;
        $display("Test Case 2 - Subtraction: A=%h, B=%h, F=%h, FR=%b", 
                 uut.alu_a, uut.alu_b, uut.F, FR);
                 
        // Test Case 3: Bitwise AND
        // Load A with 0xF0F0
        sw = 32'h0000F0F0;
        #5 clk_A = 1;
        #5 clk_A = 0;
        
        // Load B with 0x0FF0
        sw = 32'h00000FF0;
        #5 clk_B = 1;
        #5 clk_B = 0;
        
        // Set operation to AND (0111)
        sw = 32'h70000000;
        #5 clk_F = 1;
        #5 clk_F = 0;
        
        // Result should be 0x00F0 and ZF=0, SF=0, OF=0, CF=0
        #10;
        $display("Test Case 3 - AND: A=%h, B=%h, F=%h, FR=%b", 
                 uut.alu_a, uut.alu_b, uut.F, FR);
                 
        // Test Case 4: Logical Shift Left
        // Load A with 0x1
        sw = 32'h00000001;
        #5 clk_A = 1;
        #5 clk_A = 0;
        
        // Load B with 4 (shift by 4)
        sw = 32'h00000004;
        #5 clk_B = 1;
        #5 clk_B = 0;
        
        // Set operation to SLL (0001)
        sw = 32'h10000000;
        #5 clk_F = 1;
        #5 clk_F = 0;
        
        // Result should be 0x10 and ZF=0, SF=0, OF=0, CF=0
        #10;
        $display("Test Case 4 - Shift Left: A=%h, B=%h, F=%h, FR=%b", 
                 uut.alu_a, uut.alu_b, uut.F, FR);
                 
        // Test Case 5: Set Less Than (signed)
        // Load A with -10 (negative number)
        sw = 32'hFFFFFFF6; // -10 in two's complement
        #5 clk_A = 1;
        #5 clk_A = 0;
        
        // Load B with 5 (positive number)
        sw = 32'h00000005;
        #5 clk_B = 1;
        #5 clk_B = 0;
        
        // Set operation to SLT (0010)
        sw = 32'h20000000;
        #5 clk_F = 1;
        #5 clk_F = 0;
        
        // Result should be 1 (since -10 < 5) and ZF=0, SF=0, OF=0, CF=0
        #10;
        $display("Test Case 5 - SLT: A=%h, B=%h, F=%h, FR=%b", 
                 uut.alu_a, uut.alu_b, uut.F, FR);
                 
        // Test Case 6: Set to Zero (test ZF flag)
        // Load A with 25
        sw = 32'h00000019;
        #5 clk_A = 1;
        #5 clk_A = 0;
        
        // Load B with 25
        sw = 32'h00000019;
        #5 clk_B = 1;
        #5 clk_B = 0;
        
        // Set operation to SUB (1000) - should result in zero
        sw = 32'h80000000;
        #5 clk_F = 1;
        #5 clk_F = 0;
        
        // Result should be 0 and ZF=1, SF=0, OF=0, CF=0
        #10;
        $display("Test Case 6 - Zero Result: A=%h, B=%h, F=%h, FR=%b", 
                 uut.alu_a, uut.alu_b, uut.F, FR);
                 
        // Test Case 7: Negative Result (test SF flag)
        // Load A with 10
        sw = 32'h0000000A;
        #5 clk_A = 1;
        #5 clk_A = 0;
        
        // Load B with 20
        sw = 32'h00000014;
        #5 clk_B = 1;
        #5 clk_B = 0;
        
        // Set operation to SUB (1000) - should result in negative number
        sw = 32'h80000000;
        #5 clk_F = 1;
        #5 clk_F = 0;
        
        // Result should be -10 (0xFFFFFFF6) and ZF=0, SF=1, OF=0, CF=1
        #10;
        $display("Test Case 7 - Negative Result: A=%h, B=%h, F=%h, FR=%b", 
                 uut.alu_a, uut.alu_b, uut.F, FR);
                 
        // Finish simulation
        #20;
        $finish;
    end

endmodule
