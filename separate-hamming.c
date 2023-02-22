#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "hamming.h"

uint32_t redundantBitCount(uint32_t size){
  /* 
   Returns The total length of the encoded message.
   Formula : No. of Data Bits + No. of Redundant Bits + 1 (Parit Bit)
    */
  uint32_t i = 0;
  uint32_t p = 1;
  while(p <= i+size){
    p<<=1;
    i++;
  }
  return i + 1;
}

uint32_t redundantBitValuesSeparate(uint32_t *n, uint32_t redCount,  uint32_t pBit  ){
  /*
    Returns The Redundant Bit value for given Bit Location (Power of 2)
    */
  uint32_t res = 0;
  uint32_t i = 0;
  uint32_t p = 1;

  while (i < redCount){
    // X-ORs every bit whose location has binary bit set same as pBit.
    if (i+p+1 == 1<<(p) ){
      p++;
    }
    if ((i+1+p) & pBit && i+1+p != pBit){
      res ^= n[i];
    }
    i++;
  }
  return res;
}

uint32_t *encodeHammerParity(uint32_t a[], uint32_t size, uint32_t redCount){
  /*
    Returns An Encoded Hamming code array of given data bits.
    */
  uint32_t *n = (uint32_t *) malloc(sizeof(int) * (redCount - (size ) ));

  uint32_t i = 0;
  uint32_t s = 0;
  uint32_t ptrack = 1;
  uint32_t pBits = 0;

  while(ptrack <= i+size){
    n[i] = redundantBitValuesSeparate(a, size, ptrack);
    pBits ^= n[i];
    ptrack<<=1;
    i++;
  }

  // 3. Last Bit, Parity bit value assignment.
  n[redCount - size] = pBits;

  return n;
} 


uint32_t parityCheckerSeparate(uint32_t n[] ,uint32_t size, uint32_t p[], uint32_t count){
  uint32_t ptrack = 1;
  uint32_t pos = 0;

  // XOR of Parity Bit and Second Last Data Bit 
  uint32_t parity = n[size - 1] ^ p[count - 1];

  // XOR of All Bits in the Sent Data 
  parity ^= redundantBitValuesSeparate(n, size , size + count - 1);

  // XOR of All Bits in the Sent Parity Bits 
  parity ^= redundantBitValuesSeparate(p, count, size + count - 1);
  uint32_t i = 0;

  while (ptrack < size + count ){

    // Comparing Parity Bit Values passed vs Parity Bit Values Calculated.
    if (p[i] ^ redundantBitValuesSeparate(n, size, ptrack)){
      pos += ptrack;
    }
    i++;
    ptrack<<=1;
  }

  printf("\n\nparity bit: %d\tposition: %d\n", parity, pos);

  // Correcting / Flipping the Possible Error Bits
  if (pos > 0 && parity == 1){
    if (pos !=0 && (pos & (pos-1))==0){
      i = 0;
      while (pos > 0){ 
        i++;
        pos>>= 1;
      }
      p[i-1] ^= 1;

    } else{

      i = pos-1;
      while (pos > 0){ 
        i--;
        pos>>= 1;
      }
      n[i] ^= 1;

    }
    printf("Single Error Detected and corrected!");
    return 1;

  } else if (pos > 0 && parity == 0){
    printf("Double Error Detected!");
    return 3;
  } 
  else{
    printf("No Error Detected!");
    return 0;
  }

} 
