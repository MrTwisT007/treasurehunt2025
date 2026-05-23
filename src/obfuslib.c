#include<stdlib.h>
#include<stdio.h>
#include"obfuslib.h"
#include"printlib.h"

// Obfuscate
unsigned char* obfuscate(unsigned char* buffer, size_t buff_len, obfusparamstruct *obfusparam) {

   // Allocate space for obfuscated contents
   obfusparam->obfuslen = buff_len * (1 + (obfusparam->padding == 'r' || obfusparam->padding == 'z'));
   unsigned char* buffer_o = malloc(sizeof(unsigned char)*obfusparam->obfuslen);

   DEBUG_PRINT("Obfuscating contents...\n");
   for (size_t i=0; i < buff_len; i++) {
      switch (obfusparam->scheme) {
         case 'o':
            if (i%2==0) {
               *buffer_o++ = *buffer++ + obfusparam->offset1;
            } else {
               *buffer_o++ = *buffer++ + obfusparam->offset2;
            }
            break;
         case 'x':
            *buffer_o++ = *buffer++ ^ obfusparam->xor_key;
            break;
         default:
            break;
      }
      switch (obfusparam->padding) {
         case 'r':
            *buffer_o++ = rand();
            break;
         case 'z':
            *buffer_o++ = 0x00;
            break;
         default:
            break;
      }
   }
   // Rewind pointer to beginning
   buffer_o = buffer_o-obfusparam->obfuslen;
   DEBUG_PRINT("Done!\n");
   return buffer_o;
}

// Deobfuscate
unsigned char* deobfuscate(unsigned char* buffer, size_t buff_len, obfusparamstruct *obfusparam) {

   // Allocate space for deobfuscated contents
   obfusparam->obfuslen = buff_len / (1 + (obfusparam->padding == 'r' || obfusparam->padding == 'z'));
   unsigned char* buffer_d = malloc(sizeof(unsigned char)*obfusparam->obfuslen);

   //DEBUG_PRINT("Deobfuscated hex printout:\n");
   for (size_t i=0; i < obfusparam->obfuslen; i++) {
      //if (i%16==0){
      //   DEBUG_PRINT("\n");
      //}
      switch (obfusparam->scheme) {
         case 'o':
            if (i%2==0) {
               *buffer_d++ = *buffer++ - obfusparam->offset1;
            } else {
               *buffer_d++ = *buffer++ - obfusparam->offset2;
            }
            break;
         case 'x':
            *buffer_d++ = *buffer++ ^ obfusparam->xor_key;
            break;
         default:
            break;
      }
      //DEBUG_PRINT("%02x ", *(buffer_d-1));
      switch (obfusparam->padding) {
         case 'r':
            (void) *buffer++; //cast to void to shut the compiler up about unused variables
            break;
         case 'z':
            (void) *buffer++; //cast to void to shut the compiler up about unused variables
            break;
         default:
            break;
      }
   }
   //DEBUG_PRINT("\n");
   // Rewind deobfuscated buffer pointer;
   buffer_d=buffer_d-obfusparam->obfuslen;
   return buffer_d;
}
