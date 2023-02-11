#include <stdio.h>
#include <stdlib.h>
#include "hamming.h"

int main(){
  int a[4] = {1, 5, 3, 2};
  int size = 4;
  int count = redundantBitCount(size );
  int *encoded_msg = encodeHammer(a, size, count);
  printArray(encoded_msg, count);

}

