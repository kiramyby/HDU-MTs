// mdr 数据寄存器
module mdr(
    rst_n, clk_dm_n, MDR_o, MDR_i
);
    input [31:0] MDR_i; 
    input rst_n;          
    input clk_dm_n;       // 反相时钟信号
    output [31:0] MDR_o; // MDR数据

    reg_b32 mdr_reg(
        .clk(clk_dm_n),
        .rst_n(rst_n),
        .d(MDR_i),
        .q(MDR_o)
    );
endmodule