module reg_b4 (
    input   wire    clk,
    input   wire    rst_n,
    input   wire    [3:0] d,
    output  reg     [3:0] q
);

always @ (negedge rst_n or posedge clk)
begin
    if (!rst_n)
        q <= 4'b0;
    else
        q <= d;
end

endmodule