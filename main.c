#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"

int main(){
  int a[4] = {1, 0, 1, 1};
  int size = 4;
  
  // Length of the Encoded Message
  int count = hammingLength(size );
  int parityCount = redundantBitCount(size);

  // Encoding data bits {a[]} with Hamming code
  int *encoded_msg = encodeHammerParity(a, size, count);


  printf("\nEncoded Message: ");
  printArray(encoded_msg, parityCount);

  // Adding an Error in the message
  encoded_msg[2] ^= 1;
  printf("\nEncoded Message with error: ");
  printArray(encoded_msg, parityCount);

  // Checking for Error Bits and trying to correct Single bit errors.
  if (parityCheckerSeparate(a, size, encoded_msg, parityCount ) != 3){
    
    printf("\n\nDecoded Message");
    printArray(a, size);

  }



}

