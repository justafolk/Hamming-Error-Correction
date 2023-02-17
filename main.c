#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"

int main(){
  int a[4] = {1, 0, 1, 1};
  int size = 4;
  
  // Length of the Encoded Message
  int count = redundantBitCount(size );

  // Encoding data bits {a[]} with Hamming code
  int *encoded_msg = encodeHammer(a, size, count);

  printf("\nEncoded Message: ");
  printArray(encoded_msg, count);

  // Adding an Error in the message
  encoded_msg[1] = 0;
  printf("\nEncoded Message with error: ");
  printArray(encoded_msg, count);

  // Checking for Error Bits and trying to correct Single bit errors.
  if (parityChecker(encoded_msg, count) != 3){
    
    // Decoding Hamming Encoded message
    int *d = decoder(encoded_msg, count);
    printf("\n\nDecoded Message");
    printArray(d, size);

  }



}

