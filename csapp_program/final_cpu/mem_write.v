// Mem_Write 数据存储器
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

    MemRAM instMemRAM(
        .clka(clk_dm),
        .addra(DM_Addr[7:2]),  // 6位地址，寻址64个32位字
        .dina(M_W_Data),       // 写入数据
        .wea(Mem_Write),       // 写使能信号
        .douta(M_R_Data)       // 读出数据
    );

endmodule