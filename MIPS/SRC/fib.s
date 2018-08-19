.data
.text
main:

	#--------------------------------------#
	addi $a0,$zero,25
	#--------------------------------------#

	jal fib                 #jump to fib
	li   $v0, 10               # program stop
	syscall
   
fib:
	

	#--------------------------------------#
	#a0=n
	#if (n==0) return 0;
	#if (n==1) return 1;
	#return( fib(n-1)+fib(n-2) );
	addi $sp,$sp,-12 #save in stack
	sw $ra,0($sp)
	sw $s0,4($sp)
	sw $s1,8($sp)
	add $s0,$a0,$zero # s0 = a0
	addi $t1,$zero,1 # t1 = 1
	beq $s0,$zero,Return0  # if s0 = 0 return 0
	beq $s0,$t1,Return1  # if s0 =  1    return 1
	addi $a0,$s0,-1 #   n - 1  

	jal fib
	add $s1,$zero,$v0     #s1=fib(n-1)
	addi $a0,$s0,-2
	jal fib               #v0=fib(n-2)
	add $v0,$v0,$s1       #v0=fib(n-2)+$s1=fib(n-2)+fib(n-1)
	lw $ra,0($sp)       #read registers from stack
	lw $s0,4($sp)
	lw $s1,8($sp)
	addi $sp,$sp,12       #bring back stack pointer
	add $t0,$v0,$zero
	jr $ra

	Return1:
 	addi $v0,$zero,1
 	lw $ra,0($sp)       #read registers from stack
	lw $s0,4($sp)
	lw $s1,8($sp)
	addi $sp,$sp,12       #bring back stack pointer
	add $t0,$v0,$zero
	jr $ra

	Return0 :     
 	addi $v0,$zero,0
	lw $ra,0($sp)       #read registers from stack
	lw $s0,4($sp)
	lw $s1,8($sp)
	addi $sp,$sp,12       #bring back stack pointer
	add $t0,$v0,$zero
	jr $ra

	#--------------------------------------#	
    jr $ra 
