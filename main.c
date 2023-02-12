#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"

int main(){
  int a[4] = {1, 0, 1, 1};
  int size = 4;
  int count = redundantBitCount(size );
  int *encoded_msg = encodeHammer(a, size, count);
  printArray(encoded_msg, count);
    printf("\n");
  encoded_msg[6] = 0;
  printArray(encoded_msg, count);
    printf("\n");

  switch (parityChecker(encoded_msg, count)){
    case 0:
      printf("\n!!!!No Loss!!!!\n");
      break;
    case 1:
      printf("\n!!! Multiple error bits!!!");
      break;
    case 2:
      printf("\n!!!!Loss detected in Parity Bits, Data bits safe to decode!!!!\n");
      break;
    case 3:
      printf("\n!!!!Loss detected & cleared!!!!\n");
      break;
  }
  printArray(encoded_msg, count);

}

