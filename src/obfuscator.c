#include<stdlib.h>
#include<stdio.h>
#include"obfuslib.h"
#include"filereaderwriter.h"
#include"printlib.h"

int main( int argc, char *argv[] )
{
   // Pointer to read-in buffer
   unsigned char * buffer;
   size_t file_len;

   // Obfuscation parameters
   obfusparamstruct obfusparam_a;
   obfusparamstruct *obfusparam = &obfusparam_a;
   obfusparam->scheme  = 'o';
   obfusparam->padding = 'r';
   obfusparam->xor_key = 0xFF;
   obfusparam->offset1 = 2;
   obfusparam->offset2 = -3;

   if (2 >= argc) {
      DEBUG_PRINT("Need input and output files!\n");
      return 0;
   }

   // Read data and allocate a buffer
   if (argv[1] != NULL) {
      file_len = read_data(argv[1], &buffer);
      if (argv[2] != NULL) {
         if (argv[3] != NULL) { obfusparam->padding = argv[3][0]; }
         if (argv[4] != NULL) { obfusparam->scheme  = argv[4][0]; }
         if (argv[5] != NULL) { 
            switch (obfusparam->scheme) {
               case 'o':
                  obfusparam->offset1 = strtol(argv[5], NULL, 10); 
                  obfusparam->offset2 = strtol(argv[6], NULL, 10); 
                  break;
               case 'x':
                  obfusparam->xor_key = strtol(argv[5], NULL, 16);
                  break;
               default:
                  break;
            }
         }
         
         unsigned char * obf = obfuscate(buffer, file_len, obfusparam);
         write_output(argv[2], obf, obfusparam->obfuslen);
         free(obf);
#ifdef DEBUG
         free(deobfuscate(obfuscate(buffer, file_len, obfusparam), obfusparam->obfuslen, obfusparam));
#endif
      }
      free(buffer);
   }
   DEBUG_PRINT("Obfuscator done!\n");
   return 0;
}
