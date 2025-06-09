module top_cpu(
    clk, rst_n, choose,
    which, seg
);

    input clk;
    input rst_n;

    input [1:0] choose; // 选择显示的内容
    // 数码管显示
    output [7:0] seg;
    output [2:0] which;

    // 输出数据
    reg [31:0] display_data;
    
    // cpu 数据输出
    wire [31:0] PC_out;
    wire [31:0] IR_out;
    wire [31:0] MDR_out;
    wire [31:0] W_Data_out;

    // 根据选择的内容，决定显示的数据
    always @(*) begin
        case (choose)
            2'b00: display_data = PC_out;    // 显示PC
            2'b01: display_data = IR_out;    // 显示IR
            2'b10: display_data = MDR_out;   // 显示MDR
            2'b11: display_data = W_Data_out; // 显示W_Data
            default: display_data = 32'h00000000;
        endcase
    end

    cpu CPU(
        .clk(clk),
        .rst_n(rst_n),
        .PC_out(PC_out),
        .IR_out(IR_out),
        .MDR_out(MDR_out),
        .W_Data_out(W_Data_out)
    );

    display display_inst (
        .clk(clk),
        .data(display_data),  // 使用转换后的数据格式
        .which(which),
        .seg(seg)
    );

endmodule