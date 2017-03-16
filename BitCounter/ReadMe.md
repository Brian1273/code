# Brian Dunn
# Homework 2

In this assignment, I wrote C code to count the number of 0 bits and 1 bits at each bit position in the bytes of a file presented on standard input. The file is named bitcount.c and the executable bitcount.

I took each byte of standard input in turn, and for each bit position 0..7 in that byte (with bit 0 being the least-significant / rightmost bit), increment the ones-count if there is a one bit in that position, and a zero-bit otherwise. When processing is done, the program reports the number of bytes read and the number of 0 and 1 bits at each position.

I ran my program over the concatenation of all the files in /usr/bin on the schools Linux box, using the Bourne shell in the obvious way.

### Deliverables
When the testing is finished, the counter program presents a report consisting of:

- Number of bytes read/processed.
- Number of ones for each bit position.
- Number of zeroes for each bit position.

### Design
The program design was straight forward. Take a byte from STDIN. Mask each bit position to get its value. Use that value to either increment the zeroes or ones  bin in a 2D array. When all the bytes have been processed display the results.

### Testing
Testing reveals that the program exits at return with segmentation fault. This was fixed by turning on optimization. The fault only shows on exit from program. Valgrind found no errors on run.

### Questions to answer :

##### Build your program with -g. How long does your program take to run? What is the processing rate in bytes per second?
- Using the full /usr/bin/ path the program takes 1:21.32 elapsed. Processing 1.17 Gigabytes this results in (1172976858 bytes/ 81.32secs = 14 Megabytes/sec
        
##### Now, run it a second time. Is it faster? If so, why?
- The second run was slightly faster. I think that the data written on the first run doesnt need to be written again on the second run.

##### Now, build with -O4 to turn on the GCC optimizer. Run it again. How much faster is it?
- The system time is only slightly faster but the user time has greatly reduced.

##### Do you notice anything interesting about the output? If so, can you explain it?
-The results consistantly show that there are more zeroes than ones. THis implies that there are alot of unused bits. The ranges that different data types CAN use are far greater than what they actually use.


### Example Runs
#### Normal Run(Standard Compile)
dunn3@ada:~/Documents/winter2016/cs201/HW_2$ cat /usr/bin/* |time ./bitcount
cat: /usr/bin/passwd.local: Permission denied
cat: /usr/bin/wall: Permission denied
cat: /usr/bin/X11: Is a directory

    bytes processed : 1172976858
    
    bit 0:  771035090 zeros, 401941768 ones
    
    bit 1:  852348132 zeros, 320628726 ones
    
    bit 2:  799429289 zeros, 373547569 ones
    
    bit 3:  745320622 zeros, 427656236 ones
    
    bit 4:  888895420 zeros, 284081438 ones
    
    bit 5:  806302610 zeros, 366674248 ones
    
    bit 6:  718329365 zeros, 454647493 ones
    
    bit 7:  860690335 zeros, 312286523 ones
    87.78user 0.77system 1:29.54elapsed 98%CPU (0avgtext+0avgdata 476maxresident)k
    24inputs+0outputs (0major+159minor)pagefaults 0swaps
    dunn3@ada:~/Documents/winter2016/cs201/HW_2$ cat /usr/bin/* |time ./bitcount

#### Run with smaller file
    dunn3@ada:~/Documents/winter2016/cs201/HW_2$ cat /usr/bin/c*.* |time ./bitcount
    
    bytes processed : 34817975
    
    bit 0:  24718464 zeros, 10099511 ones
    
    bit 1:  26811860 zeros, 8006115 ones
    
    bit 2:  25810349 zeros, 9007626 ones
    
    bit 3:  23303510 zeros, 11514465 ones
    
    bit 4:  27069269 zeros, 7748706 ones
    
    bit 5:  25714577 zeros, 9103398 ones
    
    bit 6:  23306262 zeros, 11511713 ones
    
    bit 7:  28372559 zeros, 6445416 ones
    2.50user 0.01system 0:02.54elapsed 99%CPU (0avgtext+0avgdata 472maxresident)k
    0inputs+0outputs (0major+160minor)pagefaults 0swaps
    dunn3@ada:~/Documents/winter2016/cs201/HW_2$
#### Run with debugging flag for compile
    dunn3@ada:~/Documents/winter2016/cs201/HW_2$ cat /usr/bin/c*.* |time ./bitcount
    
    bytes processed : 34817975
    
    bit 0:  24718464 zeros, 10099511 ones
    
    bit 1:  26811860 zeros, 8006115 ones
    
    bit 2:  25810349 zeros, 9007626 ones
    
    bit 3:  23303510 zeros, 11514465 ones
    
    bit 4:  27069269 zeros, 7748706 ones
    
    bit 5:  25714577 zeros, 9103398 ones
    
    bit 6:  23306262 zeros, 11511713 ones
    
    bit 7:  28372559 zeros, 6445416 ones
    2.47user 0.02system 0:02.53elapsed 98%CPU (0avgtext+0avgdata 476maxresident)k
    24inputs+0outputs (0major+159minor)pagefaults 0swaps
    dunn3@ada:~/Documents/winter2016/cs201/HW_2$
    

