module mdr(
    M_R_Data, rst_n, clk_dm_n, MDR_data
);
    input [31:0] M_R_Data; 
    input rst_n;          
    input clk_dm_n;       // 反相时钟信号
    output [31:0] MDR_data; // MDR数据

    reg_b32 mdr_reg(
        .clk(clk_dm_n),
        .rst_n(rst_n),
        .d(M_R_Data),
        .q(MDR_data)
    );
endmodule