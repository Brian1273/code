/*
Brian Dunn
Saly Hakkoum
ECE 486
Team #6

This is the implementation of  L1 Data Cache simulator for 32-bit computer with Byte addressable that has:
	1) 4-way set associative cache; 32 Byte lines;
	2) 1024 sets; uses LRU for replacement policy;
	3) uses write-allocate or write-back policy for interaction with main memory.

This implementation has 3 debug command modes:
	1) -v to display version of the code and exits execution.
	2) -t to echo the input trace from the time -t is encountered until the end.
	3) -d to dump all relevant info after simulation run from the time -d is encountered until the end.
					
Program flow:
	1) Initialize cache
	2) Get memory access instruction from input
	3) Check cache for a hit using the Check_Cache function:
		IF a hit,  return cache line number where the address is found
		IF a miss, return cache line number where memory should be placed 
			LRU:
				IF line is occupied and the Dirty bit is set, the line must be written back to memory
				Replace cache line Data with new data from memory and write Tag
	4) Continue processing requests until EOF, or debug command is entered.

Version revisions:
	Revision# 	Version	Date		Description
	  v1.0		 02/20/2017		 Implement reading from the input trace and directly accessing memory from  main memory.	
	  v1.1		 02/23/2017		 Implement Direct mapped cache for all 1024 sets.
	  v1.2		 02/29/2017		 Extend the implementation to 4 way set associative with LRU.
	  v1.3		 02/30/2017		 Implement the write_back policy using the Dirty bit.
	  v1.4		 03/02/2017		 Implement the Debug Commands and verify if they are operational.
	  v2.0		 03/05/2017		 Final Release Version.

Inputs Used:
1.	trace1.txt
2.	trace2.txt
3.	trace3.txt
4.	HW1_output.txt

*/

// Input trace file name
#define INPUT_TRACE		"HW1_output.txt"

// File Debug definitions
#define TRACE_OUT		TRUE

// Version definition
#define VERSION			2.0

// Cache Definitions 
#define NO_OF_SETS		1024
#define NO_OF_LINES		4
#define SIZE_OF_LINE		32

// Regular Definitions for readability
#define uint_64			unsigned long int
#define uint_32			unsigned int
#define uint_8			unsigned char
#define TRUE			1
#define FALSE			0

// Main Memory Constants
#define OFFSET_MASK		0x0000001F
#define INDEX_MASK		0x00007FE0
#define TAG_MASK			0xFFFF8000
#define OFFSET_VALUE		0
#define INDEX_VALUE		5
#define TAG_VALUE		15

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Functions declarations
uint_8 Check_Cache(uint_32 Tag, uint_32 Index, bool *Hit_Flag, bool *Miss_Flag);
void Set_LRU (uint_8 Line_Number, uint_32 Index, bool *Hit_Flag, bool *Miss_Flag);

// Structure Definition
struct Cache_Address
{
	uint_8		LRU: 2;
	uint_8		Valid: 1;
	uint_8		Dirty: 1;
	uint_8		Data[SIZE_OF_LINE];
	uint_32         Tag: 17;
};

// Global variable declarations
struct Cache_Address L1Cache[NO_OF_SETS][NO_OF_LINES];

/* 
Program's main function will control the flow from extracting arguments from input trace file to implementing simulator logic
and once the execution is completed, it gives out the statistics of how many clock cycles did it
take to complete the whole execution, hit count, miss count, read count, write count, etc.
*/
void main (void)
{
	// Local variable declarations
	char arg[16], addr[16];
	bool Hit_Flag = FALSE;
	bool Miss_Flag = FALSE;
	bool Version_Flag = FALSE, Debug_Flag = FALSE, Trace_Flag = FALSE;
	uint_8 Line_Number = 0;

	// Temporary variable declarations
	uint_32 Memory_Address;
	uint_32 Temp_Tag, Temp_Index, Temp_Offset;
	
	// File pointers
	FILE *fp;
	FILE *fp_out;
	
	// Counts for different categories to calculate efficiency
	uint_64 Clock_Cycles = 0;
	uint_64 Read_Hits = 0, Write_Hits = 0, Read_Misses = 0, Write_Misses = 0, Reads = 0, Writes = 0;
	uint_64 Accesses = 0, Hits = 0, Misses = 0, Write_Backs = 0;
	
	// Opening input trace file in read mode and output trace file in writing mode
	fp = fopen(INPUT_TRACE, "r");
	fp_out = fopen("Output.txt", "w");
	
	// Initialize Cache
	for (uint_32 temp_li1 = 0; temp_li1 < NO_OF_SETS; temp_li1 ++)
	{
		for (uint_32 temp_li2 = 0; temp_li2 < NO_OF_LINES; temp_li2 ++)
		{
			L1Cache[temp_li1][temp_li2].Tag = 0;
			L1Cache[temp_li1][temp_li2].LRU = 0;
			L1Cache[temp_li1][temp_li2].Valid = FALSE;
			for(uint_8 temp_li3 = 0; temp_li3 < SIZE_OF_LINE; temp_li3 ++)
				L1Cache[temp_li1][temp_li2].Data[temp_li3] = 0;
		}
	}
		
	// Main Logic to read the trace and perform required modifications
	while (fscanf(fp, "%s", arg) != EOF)			// Reads arguments like 'r', 'w', "-v", "-t" and "-d"
	{
		// Handling "-v", "-t" and "-d" arguments and making sure arg contains either 'r' or 'w' only.
		while (arg[0] == '-') 
		{
			switch (arg[1])
			{
				case 'v':
				case 'V':	
					Version_Flag = TRUE;
					break;
				case 't':
				case 'T':	
					Trace_Flag = TRUE;
					break;
				case 'd':
				case 'D':	
					Debug_Flag = TRUE;
					break;
			}
			
			// If input trace file ends with "-v", "-t" and "-d", it will end the execution
			if (fscanf(fp, "%s", arg) == EOF)
			{
				break;
			}
		}
		
		// Getting address of 'r' or 'w' argument
		fscanf(fp, "%s", addr);

		// If Version_Flag is set, print version and exit execution.
		if (Version_Flag)
		{
			printf("Version %.1f\n", VERSION);
			break;
		}

		// Echo input command if trace flag is set.
		if (Trace_Flag)
		{
			printf("%s %s\n", arg, addr);
			if (TRACE_OUT)
				fprintf(fp_out, "%s %s\n", arg, addr);
		}
		Accesses ++;
		
		// Seperating Tag, Index and offset fields from the address field.
		Memory_Address 	= (uint_32) strtol (addr, NULL, 0);
		Temp_Tag 	= (Memory_Address & TAG_MASK) >> TAG_VALUE;
		Temp_Index 	= (Memory_Address & INDEX_MASK) >> INDEX_VALUE;
		Temp_Offset = Memory_Address & OFFSET_MASK;
		Line_Number 	= Check_Cache(Temp_Tag, Temp_Index, &Hit_Flag, &Miss_Flag);
		Clock_Cycles++;						// increment a clock cycle for every cache visit.
		
		// Cache hit
		if (Hit_Flag)
		{
			Hits ++;
			if ((arg[0] == 'r') || (arg[0] == 'R'))
			{
				if(Debug_Flag)				// if debug flag is set, dump relevent information
				{
					printf("cache read hit to line %d\n", Line_Number);
					if (TRACE_OUT)			// to write the same dumped information in an output trace file
						fprintf(fp_out, "cache read hit to line %d\n", Line_Number);
				}
				Read_Hits ++;
				Reads ++;
			}
			else if ((arg[0] == 'w') || (arg[0] == 'W'))
			{
				if(Debug_Flag)				// if debug flag is set, dump relevent information
				{
					printf("cache write hit to line %d\n", Line_Number);
					if (TRACE_OUT)
						fprintf(fp_out, "cache write hit to line %d\n", Line_Number);
				}
				Write_Hits ++;
				Writes ++;
				L1Cache[Temp_Index][Line_Number].Dirty = TRUE;	 // if cache hit and is a write, set dirty bit
			}
		}
		// Cache miss
		else
		{
			Misses ++;
			if ((arg[0] == 'r') || (arg[0] == 'R'))
			{
				if(Debug_Flag)				// if debug flag is set, dump relevent information
				{
					printf("cache read miss to line %d\n", Line_Number);
					if (TRACE_OUT)
						fprintf(fp_out, "cache read miss to line %d\n", Line_Number);
				}
				Read_Misses ++;
				Reads ++;
			}
			else if ((arg[0] == 'w') || (arg[0] == 'W'))
			{
				if(Debug_Flag)				// if debug flag is set, dump relevent information
				{
					printf("cache write miss to line %d\n", Line_Number);
					if (TRACE_OUT)
						fprintf(fp_out, "cache write miss to line %d\n", Line_Number);
				}
				Write_Misses++;
				Writes ++;
			}
			
			// If Dirty bit is set, write back cache line to main memory and reset dirty bit to false
			if (L1Cache[Temp_Index][Line_Number].Dirty == TRUE)
			{
				Write_Backs ++;
				if(Debug_Flag)				// if debug flag is set, dump relevent information
				{
					printf("Dirty bit is set. So writing back line %d of set: %d to main memory\n", Line_Number, Temp_Index);
					if (TRACE_OUT)
						fprintf(fp_out, "Dirty bit is set. So writing back line %d of set: %d to main memory\n", Line_Number, Temp_Index);
				}
				Clock_Cycles += 50;			// memory access time to replace the cache line to memory
				L1Cache[Temp_Index][Line_Number].Dirty = FALSE;			// reset dirty bit
			}
			
			Clock_Cycles += 50;				// cache miss penalty (memory access to get cache line)
			L1Cache[Temp_Index][Line_Number].Tag = Temp_Tag;			// write tag field when new cache line is replaced
		}
		
		// Set the LRU field for the recently accessed line of a set
		Set_LRU(Line_Number, Temp_Index, &Hit_Flag, &Miss_Flag);
		
		// Srint the accessed set information 
		if(Debug_Flag)						// if debug_flag is set, dump relevent information
		{
			printf("\t\t\tSet: %d\n", Temp_Index);
			if (TRACE_OUT)
				fprintf(fp_out, "\t\t\tSet: %d\n", Temp_Index);
			for (uint_8 temp = 0; temp < NO_OF_LINES; temp ++)
			{
				printf("Line: %d\tTag: 0x%05X\tLRU: %d\tValid: %d\tDirty: %d\n", temp, L1Cache[Temp_Index][temp].Tag, L1Cache[Temp_Index][temp].LRU, L1Cache[Temp_Index][temp].Valid, L1Cache[Temp_Index][temp].Dirty);
				if (TRACE_OUT)
					fprintf(fp_out, "Line: %d\tTag: 0x%5X\tLRU: %d\tValid: %d\tDirty: %d\n", temp, L1Cache[Temp_Index][temp].Tag, L1Cache[Temp_Index][temp].LRU, L1Cache[Temp_Index][temp].Valid, L1Cache[Temp_Index][temp].Dirty);
			}	
		}
	}
	
	// Print statistics.
	printf("Total number of clockcycles = %ld\nAccesses = %ld; Hits = %ld; Misses = %ld; Reads = %ld; Writes = %ld;\nread hits = %ld; write hits = %ld; read Misses = %ld; write misses = %ld; write backs = %ld\n", Clock_Cycles, Accesses, Hits, Misses, Reads, Writes, Read_Hits, Write_Hits, Read_Misses, Write_Misses, Write_Backs);
	printf("Miss ratio = %.4f, Average cycles per instruction = %.4f\n", ((float) Misses / Accesses), ((float) Clock_Cycles / Accesses)); 
	if (TRACE_OUT)
	{
		fprintf(fp_out, "Total number of clockcycles = %ld\nAccesses = %ld; Hits = %ld; Misses = %ld; Reads = %ld; Writes = %ld;\nread hits = %ld; write hits = %ld; read Misses = %ld; write misses = %ld; write backs = %ld\n", Clock_Cycles, Accesses, Hits, Misses, Reads, Writes, Read_Hits, Write_Hits, Read_Misses, Write_Misses, Write_Backs);
		fprintf(fp_out, "Miss ratio = %.4f, Average cycles per instruction = %.4f\n", ((float) Misses / Accesses), ((float) Clock_Cycles / Accesses)); 
	}
	
	// Closing all the opened files
	fclose(fp);
	fclose(fp_out);
}

/* 
Function to Checks if there is required memory address in any cache lines for given Index. 
This function sets Hit_Flag to TRUE and returns	line number. 
If there are empty cache lines, it sets Miss_Flag to TRUE and returns line number. 
But if there is a cache miss, it leaves Hit_Flag to FALSE and returns the least recently used line number.
*/
uint_8 Check_Cache(uint_32 Tag, uint_32 Index, bool *Hit_Flag, bool *Miss_Flag)
{
	uint_8 temp_li;
	uint_8 Line_Number = 0;
	*Hit_Flag = FALSE;
	*Miss_Flag = FALSE;
	for (temp_li = 0; temp_li < NO_OF_LINES; temp_li ++)
	{
		if (L1Cache[Index][temp_li].Valid == TRUE)
		{
			if (L1Cache[Index][temp_li].Tag == Tag)
			{
				*Hit_Flag = TRUE;
				Line_Number = temp_li;
				break;
			}
			else
			{
				if (L1Cache[Index][temp_li].LRU == 3)
				{
					Line_Number = temp_li;
				}
			}
		}
		else
		{
			*Miss_Flag = TRUE;
			Line_Number = temp_li;
			L1Cache[Index][Line_Number].Valid = TRUE;
			break;
		}
	}
	return Line_Number;
}
			
/* 
Function to check if there is a cache hit, it modifies LRU fields of the lines in a set based on recent usage.
If there is a cache miss and if Miss_Flag is set, LRUs of filled cache lines are set and 
if Miss_Flag is not set, it will increament all the LRU bit fields in a set by 1 and resets LRU bit field of the cache line num to 0.
*/
void Set_LRU (uint_8 Line_Number, uint_32 Index, bool *Hit_Flag, bool *Miss_Flag)
{
	uint_8 temp_li;
	if(*Hit_Flag)
	{
		for(temp_li = 0; temp_li < NO_OF_LINES; temp_li ++)
		{
			if(L1Cache[Index][temp_li].LRU < L1Cache[Index][Line_Number].LRU)
			{
				L1Cache[Index][temp_li].LRU ++;
			}
		}
		L1Cache[Index][Line_Number].LRU = 0;
	}
	else
	{
		if(*Miss_Flag)
		{
			for(temp_li = 0; temp_li < Line_Number; temp_li ++)
			{
				L1Cache[Index][temp_li].LRU ++;
			}
			L1Cache[Index][Line_Number].LRU = 0;
		}
		else
		{
			for(temp_li = 0; temp_li < NO_OF_LINES; temp_li ++)
			{
				L1Cache[Index][temp_li].LRU ++;
			}
		}
	}
}

