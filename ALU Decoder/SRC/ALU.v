`timescale 1ns / 1ps

module ALU( source1,source2,ALU_CTRL,result);
    input [15:0] source1;
    input [15:0] source2;
    input [3:0] ALU_CTRL;
    output [15:0] result;
	 
	   /* add your design */   
	wire [15:0] result;
	
	assign result=(ALU_CTRL==4'b0001)?source1+source2:
			(ALU_CTRL==4'b0010)?source1-source2:
			(ALU_CTRL==4'b0100)?((source1<source2)?1:0):1;
		
endmodule
