module reg_b32 (
    input  wire        clk,
    input  wire        rst_n,
    input  wire [31:0] d,
    output reg  [31:0] q
);

always @ (negedge rst_n or posedge clk)
begin
    if (!rst_n)
        q <= 32'b0;
    else
        q <= d;
end

endmodule