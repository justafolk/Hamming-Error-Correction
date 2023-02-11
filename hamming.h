void printArray(int arr[], int size);
// Encoding
int *encodeHammer(int a[], int size, int redCount);
int redundantBitCount(int size);
int redundantBitPosition(int *a);
int redundantBitValues(int *a, int redCount, int size, int pBit);

//Decoding
int parityChecker(int n[] ,int redCount);
int corrector(int *a);
int redundantBitValuesChecker(int *n, int redCount, int size,  int pBit);
