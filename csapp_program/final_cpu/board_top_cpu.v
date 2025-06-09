module board_top_cpu(
    clk, swb, sw,
    led, seg, which, enable
);

    input   clk;
    input   [1:6]    swb;
    input   [1:32]   sw;
    output  [1:32]   led;
    output  [7:0]    seg;
    output  [2:0]    which;
    output reg enable = 1;
    assign led = 32'h00000000;
    
    top_cpu TOP_CPU (
        .clk(swb[1]),        // 使用第一个开关作为时钟输入
        .rst_n(~swb[4]),     // 使用第四个开关作为复位输入
        .choose(sw[1:2]),    // 使用前两个开关选择显示内容
        .seg(seg),
        .which(which)
    );

endmodule