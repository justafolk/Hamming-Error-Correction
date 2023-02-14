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
  int i = 0;
  int p = 1;
  while(p <= i+size){
    p<<=1;
    i++;
  }
  return i + size + 1;
}


int redundantBitValues(int *n, int redCount, int size,  int pBit, int parity){
  int res = 0;
  int i = parity;
  while (i < redCount){
    if (((i+1)^pBit) == i+1 - pBit && pBit != i+1 && i+1 != pBit){
      res ^= n[i];
    }
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
      ptrack<<=1;
      i++;
      continue;
    }
    n[i] = a[s];
    s++;
    i++;
  }
  n[++i] = 0;
  i = 0;
  ptrack = 1;
  while(ptrack <= i+size){
    n[ptrack-1] = redundantBitValues(n, redCount, size, ptrack);
    ptrack<<=1;
    i++;
  }
  return n;
} 

int *encodeHammer_diff(int a[], int size, int redCount){
  int *n = (int *) malloc(sizeof(int) * redCount - size);
  int i = 0;
  int s = 0;
  while(i < red){
    n[i] = redundantBitValues(n, redCount, size, ptrack);
    i++;
  }
  return n;
} 


int parityChecker(int n[] ,int redCount){
  int i = 0;
  int s = 1;
  int ptrack = 1;
  int size = 1;
  int red = 0;
  int t = 0;
  int res = 1;
  int p = 1;
  while(ptrack<<1 <= redCount ){
    size++;
    ptrack <<= 1;
  }
  ptrack = 1;
  while (s < size+1){
    if (i+1 == ptrack){
      red = redundantBitValues(n, redCount, redCount, ptrack);
      t = t + (res * (n[i] ^ red));
      res *= 10;
      ptrack<<=1;
      i++;
      p ^= n[i];
      continue;
    }
    p ^= n[i];
    s++;
    i++;
  }
  res = red = i = 0;
  while (t!=0){
    red = t%10;
    t /= 10;
    res += red*(1<<i);
    i++;
  }
  if (res > 0 && p > 0){
    n[res-1] ^= 1;
    return 3;
  } else if(p > 0 && res == 0){
    return 2;
  } else if(res > 0){
    return 1;
  } else{
    return 0;
  }
} 

int *decoder(int *a, int redCount){

  int i = 0;
  int s = 0;
  int ptrack = 1;
  int size = 1;

  while(ptrack<<1 <= redCount ){
    size++;
    ptrack <<= 1;
  } 

  int *n = (int *) malloc(sizeof(int) * size + 1);
  ptrack = 1;
  while (s < size+1){
    if (i+1 == ptrack){
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
