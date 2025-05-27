`timescale 1ns / 1ps

module reg_file32_tb();
    // Clock signals
    reg clk;
    reg clk_RR;
    reg clk_F;
    reg clk_WB;
    
    // Control signals
    reg rst_n;
    reg Reg_Write;
    reg cus_enable;
    
    // Address and data signals
    reg [4:0] W_Addr;
    reg [4:0] R_Addr_A;
    reg [4:0] R_Addr_B;
    reg [3:0] ALU_OP;
    reg [3:0] cus;
    
    // Output signals
    wire [3:0] FR;
    wire [7:0] seg;
    wire [2:0] which;
    
    // Internal signals to monitor (we'll use hierarchical references)
    // reg_file32 internal data
    wire [31:0] reg_a;
    wire [31:0] reg_b;
    wire [31:0] alu_a;
    wire [31:0] alu_b;
    wire [31:0] alu_f;
    wire [31:0] F;
    wire [31:0] F_;
    
    // Instantiate the Unit Under Test (UUT)
    reg_file32_top uut (
        .clk(clk),
        .Reg_Write(Reg_Write),
        .clk_RR(clk_RR),
        .clk_F(clk_F),
        .clk_WB(clk_WB),
        .W_Addr(W_Addr),
        .ALU_OP(ALU_OP),
        .R_Addr_A(R_Addr_A),
        .R_Addr_B(R_Addr_B),
        .FR(FR),
        .seg(seg),
        .which(which),
        .rst_n(rst_n),
        .cus_enable(cus_enable),
        .cus(cus)
    );
    
    // Connecting to internal signals for monitoring
    assign reg_a = uut.reg_a;
    assign reg_b = uut.reg_b;
    assign alu_a = uut.alu_a;
    assign alu_b = uut.alu_b;
    assign alu_f = uut.alu_f;
    assign F = uut.F;
    assign F_ = uut.F_;
    
    // Clock generation
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    // Test sequence
    initial begin
        // Initialize Inputs
        clk_RR = 0;
        clk_F = 0;
        clk_WB = 0;
        rst_n = 0;
        Reg_Write = 0;
        W_Addr = 0;
        R_Addr_A = 5'd1;
        R_Addr_B = 5'd2;
        ALU_OP = 0;
        cus_enable = 0;
        cus = 0;
        
        // Reset the system
        #20;
        rst_n = 1;
        
        // Test case 1: Write a value to register 1
        #10;
        W_Addr = 5'd1;      // Register 1
        cus = 4'b1010;      // Custom value
        cus_enable = 1;     // Enable custom value
        Reg_Write = 1;      // Enable register write
        #10;
        clk_WB = 1;
        #10;
        clk_WB = 0;
        // Execute write operation
        #10;
        clk_RR = 1;         // Read registers
        #10;
        clk_RR = 0;
        #10;


        W_Addr = 5'd2;      // Write to register 2
        #10;
        clk_WB = 1;
        #10;
        clk_WB = 0;
        #10;
        clk_RR = 1;         // Read registers
        #10;
        clk_RR = 0;        // Wait for ALU operation
        #10;
        clk_F = 1;          // Compute and store result
        #10;
        clk_F = 0;
        cus_enable = 0;    // Disable custom value
        #10;
        clk_WB = 1;
        #10;
        clk_WB = 0;
        Reg_Write = 0;
        
        // Wait to observe results
        #10000;
        $finish;
    end
    
    // Monitor for internal signals
    initial begin
        $monitor("Time=%0t | R_Addr_A=%0d, R_Addr_B=%0d, W_Addr=%0d | reg_a=%0h, reg_b=%0h, alu_a=%0h, alu_b=%0h | ALU_OP=%0b, alu_f=%0h | F=%0h, FR=%0b",
                 $time, R_Addr_A, R_Addr_B, W_Addr, reg_a, reg_b, alu_a, alu_b, ALU_OP, alu_f, F, FR);
    end
    
    // Display register file values at the end
    initial begin
        #290;
        $display("Final register values:");
        $display("Reg[1] = %0h", uut.reg_file32_inst.reg_file[1]);
        $display("Reg[2] = %0h", uut.reg_file32_inst.reg_file[2]);
        $display("Reg[3] = %0h", uut.reg_file32_inst.reg_file[3]);
        $display("Reg[4] = %0h", uut.reg_file32_inst.reg_file[4]);
    end
    
endmodule
