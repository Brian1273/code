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
        cmpb    $0, (%rbx)
        je      arg_parsed
	xorq    %rcx, %rcx      # set up for byte read
	movb    (%rbx), %cl     # current character in %cl
	subq    $'0, %rcx       # character to digit
	imulq   $10, %rax       # make room
	addq    %rcx, %rax      # include digit
	incq    %rbx            # point at next character
	jmp     arg_parse
arg_parsed:

	# Run the actual Collatz count. Starting value
	# (argument) is in %rax,
        # resulting length will be in %rax

# XXX YOUR ASSIGNMENT CODE GOES HERE
	xorq	%rcx, %rcx	# Stopping distance accumulation register
check:
	cmpq 	$1, %rax		# Check if at 1
	je	done
	test	$1, %al
	jnz	if_odd
if_even:
	sar	$1, %rax
	inc	%rcx
	jmp check	
if_odd:
	imulq	$3, %rax
	inc	%rax
	inc 	%rcx
	sar	$1, %rax
	inc	%rcx
	jmp check
done:	
	movq	%rcx, %rax		# put stopping distance in %rax register
	# Store the result from %rax in %rbx, with count in %rcx
	lea     output_string, %rbx     # string buffer in %rbx
	xorq    %rcx, %rcx              # count in %rcx
	xorq    %rdx, %rdx              # number of digits in %rdx
	movq    %rax, %rsi              # save a copy of target
	cmpq    $0, %rax                # handle 0 specially
	jne     count_digits
	movq    $'0, (%rbx)             # store the digit
        incq    %rcx
        jmp     finish_store
count_digits:
        call    divten                  # reduce a digit from %rax
        incq    %rdx                    # and count it
        cmpq    $0, %rax                # are we done?
	jne     count_digits
store_digits:
	movq    %rsi, %rax              # set up for next digit
        call    modten                  # find next-right digit
        addq    $'0, %rax               # digit to char
	movb    %al, -1(%rbx,%rdx)      # store char    
	incq    %rcx                    # increase count
	movq    %rsi, %rax              # set up for call
	call    divten                  # discard digit
	movq    %rax, %rsi              # put it back
	decq    %rdx                    # bump index back
	jnz     store_digits            # continue til no more digits
finish_store:
	movq    $'\n, (%rbx,%rcx)       # store the newline
	incq    %rcx
        
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
	
# Divide by ten. Subroutine due to terrible x86
# divide unit limitations. Expects dividend in %rax
divten: 
        push    %rdx
	push    %rcx
	xorq    %rdx, %rdx
        movl    $10, %ecx
        divl    %ecx
	pop     %rcx
        pop     %rdx
        ret
	
# Mod ten. Subroutine due to terrible x86
# divide unit limitations. Expects dividend in %rax
modten: 
        push    %rdx
	push    %rcx
	xorq    %rdx, %rdx
        movl    $10, %ecx
        divl    %ecx
	xorq    %rax, %rax
	movl    %edx, %eax
	pop     %rcx
        pop     %rdx
        ret

