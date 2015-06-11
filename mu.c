/*
 * mu.c
 *
 *  Created on: Jun 6, 2015
 *      Author: Anaam
 */


/**
 ** Signal conversion
 **  routines for use with Sun4/60 audio chip
 **/

#include <stdio.h>
#include "tistdtypes.h"
void linear2ulaw(/* int */);
void ulaw2linear(/* unsigned char */);

/*
** This routine converts from linear to ulaw
**
** Craig Reese: IDA/Supercomputing Research Center
** Joe Campbell: Department of Defense
** 29 September 1989
**
** References:
** 1) CCITT Recommendation G.711  (very difficult to follow)
** 2) "A New Digital Technique for Implementation of Any
**     Continuous PCM Companding Law," Villeret, Michel,
**     et al. 1973 IEEE Int. Conf. on Communications, Vol 1,
**     1973, pg. 11.12-11.17
** 3) MIL-STD-188-113,"Interoperability and Performance Standards
**     for Analog-to_Digital Conversion Techniques,"
**     17 February 1987
**
** Input: Signed 16 bit linear sample
** Output: 8 bit ulaw sample
*/

/*Anaam Ansari:
 * 
 * 06/10/2015 Changed the to process a block as oppsed to a sample.
 * 
 */

#define ZEROTRAP    /* turn on the trap as per the MIL-STD */
#define BIAS 0x84   /* define the add-in bias for 16 bit samples */
#define CLIP 0x1fff

void
linear2ulaw(sample,block,ulawbyte)
Int16* sample;
Uint8 * ulawbyte;
int block;
{
  static int exp_lut[256] = {0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
                             4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
                             5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                             5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                             7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
  int sign, exponent, mantissa;
  //unsigned char *ulawbyte;

  int i;
  for(i=0;i<block;i++)
  {

  /* Get the sample into sign-magnitude. */
  sign = (sample[i] >> 8) & 0x80;		/* set aside the sign */
  if (sign != 0) sample[i] = -sample[i];		/* get magnitude */
  if (sample[i] > CLIP) sample[i] = CLIP;		/* clip the magnitude */

  /* Convert from 16 bit linear to ulaw. */
  sample[i] = sample[i] + BIAS;
  exponent = exp_lut[(sample[i] >> 7) & 0xFF];
  mantissa = (sample[i] >> (exponent + 3)) & 0x0F;
  ulawbyte[i] = ~(sign | (exponent << 4) | mantissa);

  }
#ifdef ZEROTRAP
  if (ulawbyte[i] == 0) ulawbyte[i] = 0x02;	/* optional CCITT trap */
#endif


}

/*
** This routine converts from ulaw to 16 bit linear.
**
** Craig Reese: IDA/Supercomputing Research Center
** 29 September 1989
**
** References:
** 1) CCITT Recommendation G.711  (very difficult to follow)
** 2) MIL-STD-188-113,"Interoperability and Performance Standards
**     for Analog-to_Digital Conversion Techniques,"
**     17 February 1987
**
** Input: 8 bit ulaw sample
** Output: signed 16 bit linear sample
*/

void
ulaw2linear(ulawbyte,block,sample)
unsigned char *ulawbyte;
Int16 block;
Int16 *sample;
{


  static int exp_lut[8] = {0,132,396,924,1980,4092,8316,16764};
  int sign, exponent, mantissa,i;

  for(i=0;i<block;i++)
  {
	  ulawbyte[i] = ~ulawbyte[i];
	    sign = (ulawbyte[i] & 0x80);
	    exponent = (ulawbyte[i] >> 4) & 0x07;
	    mantissa = ulawbyte[i] & 0x0F;
	    sample[i] = exp_lut[exponent] + (mantissa << (exponent + 3));
	    if (sign != 0) sample[i] = -sample[i];

  }

}



