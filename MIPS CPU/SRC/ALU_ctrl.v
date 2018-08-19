`timescale 1ns / 1ps

module ALU_ctrl(Funct, ALU_OP, ALU_Ctrl_Out);

    input [5:0] Funct;
	input [1:0] ALU_OP;
    output wire [3:0] ALU_Ctrl_Out;
	
	////  ALUOP  ////   ////  Function  ////
	//  R-type 10  //   //   add 100000   //
	//  beq    01  //   //   sub 100010   //
	//  else   00  //   //   slt 101010   //
	/////////////////   ////////////////////
	
	///  ALUOP  Function  ALU_Ctrl  ///
	//    10     000000     0000     // default
	//    10     100000     0010     // add
	//    10     100010     0110     // sub
	//    10     101010     0111     // slt
	//    01     XXXXXX     0110     // beq
	//    00     XXXXXX     0010     // lw sw
	///////////////////////////////////
	
	/* add your code here */
	assign ALU_Ctrl_Out=(ALU_OP==2'b00)?4'b0010: //(lw sw)
					    (ALU_OP==2'b01)?4'b0110: //beq
						(ALU_OP==2'b10 && Funct==6'b100000)?4'b0010://add
						(ALU_OP==2'b10 && Funct==6'b100010)?4'b0110://sub
						(ALU_OP==2'b10 && Funct==6'b101010)?4'b0111://slt
						4'b0000;  //default
endmodule
