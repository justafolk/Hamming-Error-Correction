#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hamming.h"

void printArray(int arr[], int size){
  printf("\n");
  for(int i = 0; i < size; i++){
    printf("%d\t", arr[i]);
  }
}

int hammingLength(int size){
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
  return size + i + 1;
}


int redundantBitValues(int *n, int redCount,  int pBit  ){
  /*
    Returns The Redundant Bit value for given Bit Location (Power of 2)
    */
  int res = 0;
  int i = 0;
  while (i < redCount){
    // X-ORs every bit whose location has binary up for pBit.
    if ((i+1) & pBit && i+1 != pBit){
      res ^= n[i];
    }
    i++;
  }
  return res;
}

int *encodeHammer(int a[], int size, int redCount){
  /*
    Returns An Encoded Hamming code array of given data bits.
    */
  int *n = (int *) malloc(sizeof(int) * redCount);

  int i = 2;
  int s = 0;
  int ptrack = 4;
  int pBits = 0;
  
  // 1. Assigning data bits to non-2-power locations in the resultant array.
  while (s < size){
    if (i+1 == ptrack){
      ptrack<<=1;
      i++;
    }
    n[i] = a[s];

    pBits ^= n[i];
    s++;
    i++;
  }

  // 2. Adding The Parity Bit Values to all 2-power locations.
  i = 0;
  ptrack = 1;
  while(ptrack <= i+size){

    n[ptrack - 1] = redundantBitValues(n, redCount - 1, ptrack);

    pBits ^= n[ptrack - 1];

    ptrack<<=1;
    i++;
  }

  // 3. Last Bit, Parity bit value assignment.
  n[redCount - 1] = pBits;

  return n;
} 


int parityChecker(int n[] ,int redCount){
  int ptrack = 1;
  int pos = 0;
  
  // XOR of Parity Bit and Second Last Data Bit 
  int parity = n[redCount - 2] ^ n[redCount - 1];

  // XOR of All Bits in the Encoded Array
  parity ^= redundantBitValues(n, redCount , redCount - 1);

  while (ptrack < redCount){

      // Comparing Parity Bit Values passed vs Parity Bit Values Calculated.
      if (n[ptrack-1] ^ redundantBitValues(n, redCount, ptrack)){
        pos += ptrack;
      }
      ptrack<<=1;
  }

  printf("\n\nparity bit: %d\tposition: %d\n", parity, pos);
  
  // Correcting / Flipping the Possible Error Bits
  if (pos > 0 && parity == 1){
    n[pos-1] ^= 1;
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

int *decoder(int *a, int redCount){
  /*
    Returns the decoded message of hammer encoded message.
    */

  int i = 2;
  int s = 0;
  int ptrack = 1;
  int size = 1;

  // Calculating the length of data bits
  while(ptrack<<1 <= redCount ){
    size++;
    ptrack <<= 1;
  } 

  // Creating an Array for decoded msg
  int *n = (int *) malloc(sizeof(int) * size );
  ptrack = 4;

  // Getting all the data bits which are located in non-2-power locations.
  while (i < redCount - 1){
    if (i+1 & ptrack){
      ptrack<<=1;
      i++;
      continue;
    }
    n[s] = a[i];
    s++;
    i++;
  }
  return n;
}
