module mem_write(
    clk_dm, DM_Addr, M_W_Data, M_R_Data, Mem_Write,
    rst_n
);
    input clk_dm;
    input [31:0] DM_Addr;
    input [31:0] M_W_Data;
    output [31:0] M_R_Data;
    input rst_n;
    input Mem_Write;

    wire [31:0] DM_Data;

    // 数据存储器实例
    reg_file32 Mem(
        .clk_Regs(clk_dm),
        .R_Addr_A(DM_Addr[7:2]), // 读取地址
        .W_Addr(DM_Addr[7:2]),   // 写入地址
        .W_Data(M_W_Data),       // 写入数据
        .R_Data_A(DM_Data),      // 读取数据
        .Reg_Write(Mem_Write),     // 写使能
        .rst_n(rst_n)
    );

    assign M_R_Data = DM_Data;

endmodule