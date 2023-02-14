#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"

int main(){
  int a[4] = {1, 0, 1, 1};
  int size = 4;
  int count = redundantBitCount(size );
  int *encoded_msg = encodeHammer(a, size, count);
  printf("%d", redundantBitCount(8));
  printArray(encoded_msg, count);
  printf("\n");
  encoded_msg[6] = 0;
  printArray(encoded_msg, count);
    printf("\n");

  if (parityChecker(encoded_msg, count) == 2){
    printf("Multiple bits error!!!");
  }

  printArray(encoded_msg, count);

  int *d = decoder(encoded_msg, count);
  printf("\n");
  printArray(d, size);

}

