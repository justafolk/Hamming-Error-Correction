#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hamming.h"
#include "./flashd.h"

uint32_t *data = "1011";
uint32_t Rx_data ;
char stre[30];

int main(){
  uint32_t addr = 0x400000;
  FLASHD_Initialize(2,2);
  FLASHD_Erase(addr);
  FLASHD_Write(0x400000, data, 4);
  Rx_data = FLASHD_ReadUniqueID(0x400000);

  uint32_t a[4] = {1, 0, 1, 1};
  uint32_t size = 4;

  // Length of the Encoded Message
  uint32_t count = hammingLength(size );
  uint32_t parityCount = redundantBitCount(size);

  // Encoding data bits {a[]} with Hamming code
  uint32_t *encodedParityBits = encodeHammerParity(a, size, count);

  printf("\n\nData Bits");
  printArray(a, size);
  printf("\n\nEncoded Parity Bits");
  printArray(encodedParityBits, parityCount);

  // Adding an Error in the Data Bits


    a[2] ^= 1;
    printf("\nParity Bits with error");
    printArray(encodedParityBits, parityCount);

    // Checking for Error Bits and trying to correct Single bit errors.
    if (parityCheckerSeparate(a, size, encodedParityBits, parityCount ) != 3){
      printf("\n\nDecoded Data Bits");
      printArray(a, size);
      printf("\n\nDecoded Parity Bits");
      printArray(encodedParityBits, parityCount);
      printf("\n===================================\n");
    }


}

