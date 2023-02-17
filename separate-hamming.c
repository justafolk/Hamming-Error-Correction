#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hamming.h"

int redundantBitCount(int size){
  /* 
   Returns The total length of the encoded message.
   Formula : No. of Data Bits + No. of Redundant Bits + 1 (Parit Bit)
    */
  int i = 0;
  int p = 1;
  while(p <= i+size){
    p<<=1;
    i++;
  }
  return i + 1;
}
int redundantBitValuesSeparate(int *n, int redCount,  int pBit  ){
  /*
    Returns The Redundant Bit value for given Bit Location (Power of 2)
    */
  int res = 0;
  int i = 0;
  int p = 1;

  while (i < redCount){
    // X-ORs every bit whose location has binary up for pBit.
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

int *encodeHammerParity(int a[], int size, int redCount){
  /*
    Returns An Encoded Hamming code array of given data bits.
    */
  int *n = (int *) malloc(sizeof(int) * (redCount - (size ) ));

  int i = 0;
  int s = 0;
  int ptrack = 1;
  int pBits = 0;

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


int parityCheckerSeparate(int n[] ,int size, int p[], int count){
  int ptrack = 1;
  int pos = 0;
  
  // XOR of Parity Bit and Second Last Data Bit 
  int parity = n[size - 1] ^ p[count - 1];

  // XOR of All Bits in the Encoded Array
  printf("%d count\n", size + count - 1);
  parity ^= redundantBitValuesSeparate(n, size , size + count - 1);
  parity ^= redundantBitValuesSeparate(p, count, size + count - 1);
  int i = 0;

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
    if (pos !=0 && (pos & (pos-1))!=0){
      p[pos-1] ^= 1;
    } else{
      n[pos] ^= 1;
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
