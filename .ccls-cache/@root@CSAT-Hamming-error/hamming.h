void printArray(int arr[], int size);
// Encoding
int hammingLength(int size);
int *encodeHammer(int a[], int size, int redCount  );
int redundantBitValues(int *a, int redCount, int pBit);



//Decoding
int parityChecker(int n[] ,int redCount);
int *decoder(int *a, int redCount);

// Separate Encoding 
int *encodeHammerParity(int a[], int size, int redCount  );
int redundantBitCount(int size);
int redundantBitValuesSeparate(int *a, int redCount, int pBit);
int parityCheckerSeparate(int n[] ,int size, int p[], int count);
