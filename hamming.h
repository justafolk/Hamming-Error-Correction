#include <stdint.h>
void printArray(uint32_t arr[], uint32_t size);
// Encoding
uint32_t hammingLength(uint32_t size);
uint32_t *encodeHammer(uint32_t a[], uint32_t size, uint32_t redCount  );
uint32_t redundantBitValues(uint32_t *a, uint32_t redCount, uint32_t pBit);



//Decoding
uint32_t parityChecker(uint32_t n[] ,uint32_t redCount);
uint32_t *decoder(uint32_t *a, uint32_t redCount);

// Separate Encoding 
uint32_t *encodeHammerParity(uint32_t a[], uint32_t size, uint32_t redCount  );
uint32_t redundantBitCount(uint32_t size);
uint32_t redundantBitValuesSeparate(uint32_t *a, uint32_t redCount, uint32_t pBit);
uint32_t parityCheckerSeparate(uint32_t n[] ,uint32_t size, uint32_t p[], uint32_t count);
