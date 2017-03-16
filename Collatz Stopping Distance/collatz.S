# copyright © 2016 Brian Dunn
# Collatz total stopping time in X86-64 assembly for Linux

# Buffer for writing output. Could use the
# stack, but this is maybe easier.        
        .data
        .lcomm   output_string, 20

# The OS (actually ld-linux.so) executes a dynamically-linked
# program by setting up the registers just so and jumping
# to the address given by the global symbol _start.
	
# The main program
        .text
        .globl  _start
_start:
	
	# Parse the input integer. We assume that
	# the input is well-formatted.
        movq    16(%rsp), %rbx  # string pointer on stack
        xorq    %rax, %rax      # input integer in %rax
arg_parse:
        cmpb    $0, (%rbx)	# Check the stack to see if it empty
        je      arg_parsed	# If empty then jump to arg_parsed
	xorq    %rcx, %rcx      # set up for byte read
	movb    (%rbx), %cl     # current character in %cl
	testb	$64, %cl	# Check if digit or letter. Bit 7 is 0 for numbers, 1 for letters
	jz	digit		# IF result is zero then a digit and jump to digit
	add	$9, %cl		# The character is a letter so add 9	
digit:	
	and	$0xf, %rcx      # character to digit
	imulq   $16, %rax       # make room
	addq    %rcx, %rax      # include digit
	incq    %rbx            # point at next character
	jmp     arg_parse	# continue to parse characters
arg_parsed:

	# Run the actual Collatz count. Starting value
	# (argument) is in %rax,
        # resulting length will be in %rax

# XXX YOUR ASSIGNMENT CODE GOES HERE
	xorq	%rcx, %rcx	# Stopping distance accumulation register
check:
	cmpq 	$1, %rax	# Check if at 1
	je	done		# If we are at 1 then we are done	
	test	$1, %al		# if bit 0 is 1 then number is odd
	jnz	if_odd		# if odd go to if_odd
if_even:
	sar	$1, %rax	# Else it is even. Divide by 2 using right shift
	inc	%rcx		# Count the step
	jmp check		# Continue processing 
if_odd:
# odd number mult by 3 is odd, add 1 makes it even so a divide by 2 imediatly follows((3n+1)/2)
	imulq	$3, %rax	# Odd so multiply by three
	inc	%rax		# Add 1
	inc 	%rcx		# Count the step
	sar	$1, %rax	# Divide by 2 using right shift
	inc	%rcx		# Count the step
	jmp check		# Continue Processing
done:	
	movq	%rcx, %rax		# put stopping distance in %rax register
	# Store the result from %rax in %rbx, with count in %rcx
	lea     output_string, %rbx     # string buffer in %rbx
	xorq    %rcx, %rcx              # count in %rcx
	xorq    %rdx, %rdx              # number of digits in %rdx
	movq    %rax, %rsi              # save a copy of target
	cmpq    $0, %rax                # handle 0 specially
	jne     clear_zeros		# If not zero then go to clear_zeros
	movq    $'0, (%rbx)             # store the digit
        incq    %rcx			# count the digit	
        jmp     finish_store		# jump to finish store
# Clears the leading zeros from a result
clear_zeros:
	mov 	$16,%rdx		#byte tracker. 16 byte register.
	mov 	$0xf000000000000000, %r9	# %r9 is the byte mask.
shift:
       	testq	%rsi, %r9		# Starting with the most significant byte, check each byte for zero
	jnz	store_digits		# If the Byte is not zero then it is MSB, go to store_digit
	shr	$4, %r9			# IF the Byte IS zero then it a leading zero. Shift the mask for the next significant Byte.
	decq	%rdx			# %rdx contains the count of significant bytes. 16 byte register - each leading zero byte.
	jnz 	shift			# Keep checking until there is something in the byte or checked all 16.

store_digits:
	movq	$0xf, %r9		# Mask for Least Significant Byte
	andq	%r9, %rax		# Get the just the byte
	cmpq	$9, %rax		# Check if going to be a digit or character
	js	set			# If a number jump to set:
	addq	$7, %rax		# Else add 7 for the difference between '9 and 'a(25 for lower case)
set:
        addq    $'0, %rax               # digit to char. '0 is ascii 0
	movb    %al, -1(%rbx,%rdx)      # store char at end of array(Least Significant Digit)   
	incq    %rcx                    # increase count of characters in array
	shr	$4, %rsi		# Right shift the register for the next byte
	decq    %rdx                    # bump index back 
	movq	%rsi, %rax		# Move the new value int %rax
	jnz     store_digits            # continue til no more digits
finish_store:
	movq    $'\n, (%rbx,%rcx)       # store the newline
	incq    %rcx			# Include in character count.
        
        # Write the result from %rbx, using %rcx bytes
	movq    $1,  %rax               # write system call
	movq    $1, %rdi                # stdout
	movq    %rbx, %rsi              # buffer pointer
	movq    %rcx, %rdx              # number of bytes
	syscall
	
        # Exit the program.
        # "exit" system call number.
        movq    $60, %rax
        # Exit status argument.
        movq    $0, %rdi
        # Make a system call.
        syscall
	

