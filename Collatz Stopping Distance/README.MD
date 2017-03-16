# Brian Dunn
# Homework 3
## The Collatz Conjecture

The
[Collatz Relation](https://en.wikipedia.org/wiki/Collatz_conjecture)
is a recursive relation defined by:

        C(1) = 1
        C(n) = C(n div 2)  (n even)
        C(n) = C(3n + 1)   (n odd and greater than 1)

It is conjectured that this relation is a function with
constant value 1: that is, it is conjectured that if you
start with any positive integer and keep evaluating the
Collatz Relation long enough, you will eventually end with
1. The number of evaluation steps to reach 1 from
starting point *n* is known as the *total stopping time* of *n*.
In this assignment,The file is named collatz.S and the executable collatz.

The assignment was to write -- *in pure Linux x86-64
assembly code* -- code that prints the Collatz stopping time of a positive integer presented as an argument to the program.
By pure assembly it is meant that one may not call any C library code: code must be in assembly only.The assignment included a collatz.S file and the Makefile for the program. Within collatz.S was the indicated spot to insert MY code. 

### Part 1 (Decimal Input/output)
####    Deliverables
When the code is running, test it against some known
values.
    Try at least these:

        in              out
        1               0
        3               7
        4294967295      451
#### Design
In my research on the collatz conjecture I discovered a trick about about handling the odd numbers. If a number is odd(and not 1) then 3n is odd +1 is even which will be divided by two giving (3n+1)/2 for odd integers. This makes the jumping around less and saves loads into registers. Dividing by two is accomplished by right shifting the register. Adding 1 is just incrementing a register. Adding 0 to a register is XOR the contents with itself.

`The collatz count code flows as follows:`
1. Check if 1
2. If 1 then done if not 1 then:
3. Check if odd:
    
    If odd then multiply by three then add 1.

    Then Divide by two.
    
    Increase step count by 2.
    
    Return to check if 1
4. If even:    

    Right shift the register(divide by two).
    
    Increment the step count.
    
    Return to check if 1.
```  
        # XXX YOUR ASSIGNMENT CODE GOES HERE
        xorq    %rcx, %rcx      # Stopping distance accumulation register
check:
        cmpq    $1, %rax        # Check if at 1
        je      done            # If we are at 1 then we are done
        test    $1, %al         # if bit 0 is 1 then number is odd
        jnz     if_odd          # if odd go to if_odd
if_even:
        sar     $1, %rax        # Else it is even. Divide by 2 using right shift
        inc     %rcx            # Count the step
        jmp check               # Continue processing
if_odd:
# odd number mult by 3 is odd, add 1 makes it even so a divide by 2 imediatly follows((3n+1)/2)
        imulq   $3, %rax        # Odd so multiply by three
        inc     %rax            # Add 1
        inc     %rcx            # Count the step
        sar     $1, %rax        # Divide by 2 using right shift
        inc     %rcx            # Count the step
        jmp check               # Continue Processing
done:
```  
#### Testing
Along with the required examples I also tested some known values of numbers with large stopping distances.
#### Example Runs

![Sample Runs](https://drive.google.com/file/d/0BzMnWRMimu2pcFpiTHl3TkRPbVE/view?usp=sharing)

dunn3@ada:~/Documents/winter2016/cs201/3_hw$ make  
as --gstabs+ -o collatz.o collatz_dec.S  
ld -L/lib/x86_64-linux-gnu/ -I/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 -o collatz collatz.o -lc
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 17  
    12  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 4294967295  
    451  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 1  
    0  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 3
    7  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 1008932249296231            
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 31835572457967  
    928  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 70665924117439
    955  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 739448869367967  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 27  
    111  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$ ./collatz 9780657631  
    1132  
dunn3@ada:~/Documents/winter2016/cs201/3_hw$

### Part 2 (Hexadecimal I/O)
####    Deliverables
Modify the code so that it accepts input and prints output.  

Your tests should now look like:

        in              out
        1               0
        3               7
        ffffffff        1c3
#### Design
The existing code needs to be adjusted to so that it takes input and outputs in hexadecimal format instead of decimal (without a leading "0x").  

`Here is my Algorithm for parsing the numbers:`
```_start:

        # Parse the input integer. We assume that
        # the input is well-formatted.
        movq    16(%rsp), %rbx  # string pointer on stack
        xorq    %rax, %rax      # input integer in %rax
arg_parse:
        cmpb    $0, (%rbx)      # Check the stack to see if it empty
        je      arg_parsed      # If empty then jump to arg_parsed
        xorq    %rcx, %rcx      # set up for byte read
        movb    (%rbx), %cl     # current character in %cl
        testb   $64, %cl        # Check if digit or letter. Bit 7 is 0 for numbers, 1 for letters
        jz      digit           # IF result is zero then a digit and jump to digit
        add     $9, %cl         # The character is a letter so add 9
digit:
        and     $0xf, %rcx      # character to digit
        imulq   $16, %rax       # make room
        addq    %rcx, %rax      # include digit
        incq    %rbx            # point at next character
        jmp     arg_parse       # continue to parse characters
arg_parsed:
```  

The collatz counting section remains the same but the formating for Hex output needs to be written.  
`The code is as follows:`  
```
    done:
        movq    %rcx, %rax              # put stopping distance in %rax register
        # Store the result from %rax in %rbx, with count in %rcx
        lea     output_string, %rbx     # string buffer in %rbx
        xorq    %rcx, %rcx              # count in %rcx
        xorq    %rdx, %rdx              # number of digits in %rdx
        movq    %rax, %rsi              # save a copy of target
        cmpq    $0, %rax                # handle 0 specially
        jne     clear_zeros             # If not zero then go to clear_zeros
        movq    $'0, (%rbx)             # store the digit
        incq    %rcx                    # count the digit
        jmp     finish_store            # jump to finish store
# Clears the leading zeros from a result
clear_zeros:
        mov     $16,%rdx                #byte tracker. 16 byte register.
        mov     $0xf000000000000000, %r9        # %r9 is the byte mask.
shift:
        testq   %rsi, %r9               # Starting with the most significant byte, check each byte for zero
        jnz     store_digits            # If the Byte is not zero then it is MSB, go to store_digit
        shr     $4, %r9                 # IF the Byte IS zero then it a leading zero. Shift the mask for the next significant Byte.
        decq    %rdx                    # %rdx contains the count of significant bytes. 16 byte register - each leading zero byte.
        jnz     shift                   # Keep checking until there is something in the byte or checked all 16.
store_digits:
        movq    $0xf, %r9               # Mask for Least Significant Byte
        andq    %r9, %rax               # Get the just the byte
        cmpq    $9, %rax                # Check if going to be a digit or character
        js      set                     # If a number jump to set:
        addq    $7, %rax                # Else add 7 for the difference between '9 and 'a(25 for lower case)
set:
        addq    $'0, %rax               # digit to char. '0 is ascii 0
        movb    %al, -1(%rbx,%rdx)      # store char at end of array(Least Significant Digit)
        incq    %rcx                    # increase count of characters in array
        shr     $4, %rsi                # Right shift the register for the next byte
        decq    %rdx                    # bump index back
        movq    %rsi, %rax              # Move the new value int %rax
        jnz     store_digits            # continue until no more digits
finish_store:
        movq    $'\n, (%rbx,%rcx)       # store the newline
        incq    %rcx                    # Include in character count.

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
```

#### Testing
Along with the required examples I also tested some known values of numbers with large stopping distances.
#### Example Runs
![Hex Output](https://drive.google.com/file/d/0BzMnWRMimu2pZGdtd3cwS215VlE/view?usp=sharing)
