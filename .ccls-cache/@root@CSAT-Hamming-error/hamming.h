void printArray(int arr[], int size);
// Encoding
int *encodeHammer(int a[], int size, int redCount);
int redundantBitCount(int size);
int redundantBitPosition(int *a);
int redundantBitValues(int *a, int redCount, int size, int pBit);

//Decoding
int parityChecker(int n[] ,int redCount);
int *decoder(int *a, int redCount);
