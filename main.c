
/*
 * main.c
 * That calls
 *
 *  * void linear2ulaw( int );
 *  void ulaw2linear(unsigned char );
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "tistdtypes.h"



#include <stdio.h>
#include <stdlib.h>
#include "tistdtypes.h"

Uint8 waveHeader[44];

#define SIZE 5120
Int16 inp[SIZE/2];
Int16 out[SIZE/2];
Int8 ch[SIZE];                                 // Declare a char[1024] array for experiment
//Uint16 out;

void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes);

void main()

{

/*-------------------------------------------------------------------------------------------------*/
	FILE *fp1,*fp2,*fp3;                             // File pointers
	Uint32 i,j,k,size,n;                        	 // Unsigned long integer used as a counter

	printf("Begin Compand Experiment\n");
	fp1 = fopen("..\\C55DSPAudioTest.wav", "rb");	 // Open input file
	fp2 = fopen("..\\compressed.cod", "wb");		 // Open output file
	fp3 = fopen("..\\expanded.wav","wb");
	if (fp1 == NULL)                                 // Check if the input file exists
	{
		printf("Failed to open input file 'C55DSPUSBStickAudioTest.pcm'\n");
		exit(0);
	}
	n=0;

	  fread(waveHeader, sizeof(Int8), 44, fp1);
	  fwrite(waveHeader,sizeof(Int8), 44, fp3);

	while (fread(ch, sizeof(Uint8), SIZE, fp1) == SIZE) // Read in SIZE of input data bytes
	{

	// Convert 2 bytes to a word from PCM data
	for(i=0;i<SIZE/2;i++)
		{
		inp[i]=(ch[2*i]&0xFF)|(ch[2*i+1]<<8);
		}
	//compress
	linear2ulaw(inp,SIZE/2,ch);

	//write compressed data
	fwrite(ch ,sizeof(Int16),SIZE/2,fp2);




	// expand
	ulaw2linear(ch,SIZE/2,out);

	//Unroll the output word in bytes to PCM data
	for (i=0;i<SIZE/2;i++)           			// Get PCM input data
	        {
	        	ch[2*i] = out[i]&0xFF;
	        	ch[2*i+1] = (out[i]>>8)&0xFF;
	        }

	//Writing the output expanded wave
	fwrite(ch ,sizeof(Int8),SIZE,fp3);


		n += SIZE/2;
		printf("%ld words processed\n", n);    // Show the number of data is processed

	}

	 wHeader(waveHeader, 8000, 8000, n<<1);
	 rewind(fp3);
	fwrite(waveHeader, sizeof(Int8), 44, fp3);


	fclose(fp1);                                // Close input file
	fclose(fp2);                              // Close output file
	fclose(fp3);
	printf("\nCompand Complete\n");
}
/*-------------------------------------------------------------------------------------------------*/

// Create the wave file header
void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes)
{
    Int32 t;

    t = bytes;
    w[40] = (t>>0)&0xff;	// Set up output file size
    w[41] = (t>>8)&0xff;
    w[42] = (t>>16)&0xff;
    w[43] = (t>>24)&0xff;
    t += 36;
    w[4] = (t>>0)&0xff;
    w[5] = (t>>8)&0xff;
    w[6] = (t>>16)&0xff;
    w[7] = (t>>24)&0xff;
    t = w[24]|(w[25]<<8)|((Int32)w[26]<<16)|((Int32)w[27]<<24);
    t = (Int32)((float)t*f2/f1);
    w[24] = (t>>0)&0xff;	// Set up output file frequency
    w[25] = (t>>8)&0xff;
    w[26] = (t>>16)&0xff;
    w[27] = (t>>24)&0xff;
    t = w[28]|(w[29]<<8)|((Int32)w[30]<<16)|((Int32)w[31]<<24);
    t = (Int32)((float)t*f2/f1);
    w[28] = (t>>0)&0xff;	// Set up output file byte rate
    w[29] = (t>>8)&0xff;
    w[30] = (t>>16)&0xff;
    w[31] = (t>>24)&0xff;
    return;
}




