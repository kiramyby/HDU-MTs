module reg_b4 (
    clk, rst_n, d, q
);

input         clk;
input         rst_n;
input  [3:0] d;
output reg  [3:0] q;

always @ (negedge rst_n or posedge clk)
begin
    if (!rst_n)
        q <= 4'b0;
    else
        q <= d;
end

endmodule