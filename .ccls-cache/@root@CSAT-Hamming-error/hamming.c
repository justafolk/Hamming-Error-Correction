#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hamming.h"


void printArray(int arr[], int size){
  for(int i = 0; i < size; i++){
    printf("%d\t", arr[i]);
  }
}

int redundantBitCount(int size){
  int i = 2;
  int p = 2;
  while(size > 0){
    if (p == i){
      p<<=1;
      i++;
      continue;
    }
    i++;
    size--;
  }
  return i-1;
}

int redundantBitValues(int *n, int redCount, int size,  int pBit){
  int res = 0;
  int i = 0;
  int s = 0;
  int ptrack = 1;
  while (s < size){
    if (((i+1)^pBit) == i+1 - pBit && pBit != i+1 && i+1 != ptrack){
      res ^= n[s];
      i++;
      s++;
      continue;
    }
    if (i+1 == ptrack){
      ptrack<<=1;
      i++;
      continue;
    }
    s++;
    i++;
  }
  return res;

}

int *encodeHammer(int a[], int size, int redCount){
  int *n = (int *) malloc(sizeof(int) * redCount);
  int i = 0;
  int s = 0;
  int ptrack = 1;
  while (s < size){
    if (i+1 == ptrack){
      n[i] = redundantBitValues(a, redCount, size, ptrack);
      ptrack<<=1;
      i++;
      continue;
    }
    n[i] = a[s];
    s++;
    i++;
  }
  return n;
} 
