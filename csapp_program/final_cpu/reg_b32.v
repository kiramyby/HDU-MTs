module reg_b32 (
    clk, rst_n, d, q
);
input         clk;
input         rst_n;
input  [31:0] d;
output reg  [31:0] q;

always @ (negedge rst_n or posedge clk)
begin
    if (!rst_n)
        q <= 32'b0;
    else
        q <= d;
end

endmodule