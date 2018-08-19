module traffic_light(clk,rst,pass,R,G,Y);
input clk,rst,pass;
output R,G,Y;

reg [4:0] count;
reg R,G,Y;


always@(posedge clk)
begin
    if(rst)
    begin R=1'b1; G=1'b0; Y=1'b0; count=5'b00001; end

    else
    begin

            if(count<'d10) begin R=1'b1; G=1'b0; Y=1'b0; count=count+5'b00001; end            
          
        else if(count<'d22)
		begin 
		R=1'b0; G=1'b1; Y=1'b0; count=count+5'b00001;
		end      
        else if(count<'d27)
		begin 
                R=1'b0; G=1'b0; Y=1'b1; count=count+5'b00001; 
		end          
        else  
	        begin  
		R=1'b1; G=1'b0; Y=1'b0; count=5'b00001; 
		end	
    end	
end    	

always@(posedge pass)
begin
		if(pass)
		begin
			if(!G)
			begin R=1'b0; G=1'b1; Y=1'b0; count=5'b01011; end  
		end
end
	    	
endmodule

