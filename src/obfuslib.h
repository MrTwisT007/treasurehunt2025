// Typedef
typedef struct {
   char scheme;
   char padding;
   unsigned char xor_key;
   int offset1;
   int offset2;
   size_t obfuslen;
} obfusparamstruct;

// Function declarations
unsigned char*    obfuscate(unsigned char *, size_t, obfusparamstruct *);
unsigned char*    deobfuscate(unsigned char *, size_t, obfusparamstruct *);

