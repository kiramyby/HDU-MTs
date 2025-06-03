module if(
    IR_Write, PC_Write, clk_im, rst_n,
    inst
);

    input IR_Write;
    input PC_Write;
    input clk_im;
    input rst_n;
    output [31:0] inst;

    wire [31:0] IM_Addr;      // 当前PC地址
    wire [31:0] IM_Addr_nx;   // 下一个PC地址
    wire [31:0] inst_code;

    wire clk_im_n = ~clk_im; // 反相时钟信号

    // PC自增加法器
    assign IM_Addr_nx = IM_Addr + 4;

    pc pc(
        .PC_Write(PC_Write),
        .clk_im_n(clk_im_n),
        .IM_Addr(IM_Addr),
        .IM_Addr_nx(IM_Addr_nx),
        .rst_n(rst_n)
    );

    InstROM instROMInst(
        .clka(clk_im),
        .addra(IM_Addr[7:2]),
        .doubta(inst_code)
    );

    ir ir(
        .clk_im(clk_im),
        .IR_Write(IR_Write),
        .inst_code(inst_code),
        .inst(inst),
        .rst_n(rst_n)
    );
endmodule