/*	Copyright (c) Brian Dunn	*/
/*	Bit Counter			*/


#include <stdio.h>
#include <stdlib.h>




int main(int argc, char **argv){

//Declare and initialize variables
	unsigned char buffer =0;	//buffer to store data
	size_t reads =0;		//Return variable for fread()
	long bytes_processed =0;	//Bytes processed from stdin
	unsigned long results[8][2];	//2D Array to store bit results
//Initialize results array and fill it with zeroes
	for(int m=0; m<9; m++){
		for(int n=0; n<2; n++){
			results[m][n]= 0;
		}
	}
//Read data from STDIN
//Continue while not at the end of the file
	while(!feof(stdin)){		//feof() checks for end of file flag
//Move a byte of data into buffer. fread() returns bytes processed
		reads =	fread(&buffer, 1, 1, stdin);
//Loop to access each bit in the byte.
		for(int b=0; b<8; b++){
//Mask each individual bit for bits 0 - 7. Each pass shifts the mask to the next bit.
			unsigned char temp = (buffer & (1<<b));	//variable to stored masked result
			if(temp){				//If not 0, increment the ones bin
				results[b][1]++;
			}
			else{					//Must be a zero so increment the zeroes
				results[b][0]++;
			}
		}
//Increment the Bytes processed count each pass.
		bytes_processed++;
	}							//End while loop
//Display results
	printf("\nbytes processed : %lu\n", bytes_processed);	
//Used for error checking if fread() fails and to use the variable to eliminate compiler errors.
	if(reads){
		printf("\nreads unprocessed : %lu\n", reads);
	}
//Loop to display results 2D array
	for(int j=0; j<8; j++){
	        printf("\nbit %d:  %lu zeros, %lu ones\n", j, results[j][0], results[j][1]);
	}
//exit with 0 status.
	return 0;
}

