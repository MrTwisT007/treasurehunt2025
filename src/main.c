#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"obfuslib.h"
#include"printlib.h"
#include"filereaderwriter.h"

extern unsigned char secret_start;
extern unsigned char secret_byte;
extern unsigned char secret_end;
extern unsigned char payload_start;
extern unsigned char payload_end;
extern unsigned char hint_start;
extern unsigned char hint_end;
extern unsigned char dtmf_start;
extern unsigned char dtmf_end;
extern unsigned char unlock_start;
extern unsigned char unlock_end;
extern unsigned char src_start;
extern unsigned char src_end;

//IF YOU CAN SEE THIS COMMENT IN THE OBJDUMP, TURN OFF DEBUG SYMBOLS

unsigned char* secret   = &secret_start;
unsigned char* payload  = &payload_start;
unsigned char* hint     = &hint_start;
unsigned char* dtmf     = &dtmf_start;
unsigned char* unlock   = &unlock_start;
unsigned char* src      = &src_start;

obfusparamstruct obfusparam;

int main(){
   size_t payload_len   = &payload_end -  &payload_start;
   size_t hint_len      = &hint_end    -  &hint_start;
   size_t dtmf_len      = &dtmf_end    -  &dtmf_start;
   size_t unlock_len    = &unlock_end  -  &unlock_start;
   size_t src_len       = &src_end     -  &src_start;

   obfusparamstruct obfusparam_a;
   obfusparamstruct *obfusparam = &obfusparam_a;
   obfusparam->scheme  = 'o';
   obfusparam->padding = 'r';
   obfusparam->xor_key = 0xFF;
   obfusparam->offset1 = 2;
   obfusparam->offset2 = -3;

   if (secret_start == 'P') {
      obfusparam->scheme = 'o';
      obfusparam->padding = 'n';
      obfusparam->offset1 = 3;
      obfusparam->offset2 = -2;
      unsigned char * src_deobf = deobfuscate(src, src_len, obfusparam);
      stringify(src_deobf,obfusparam->obfuslen);
      write_output("src.zip", src_deobf, obfusparam->obfuslen);
      free(src_deobf);
      return 0;
   }

   if (secret_byte == 0xa5) {
      funprint("Awaiting user input: ", 30000);
      char uinput[32];
      fgets(uinput, sizeof(uinput), stdin);

      obfusparam->xor_key = uinput[7];

      unsigned char * dtmf_deobf = deobfuscate(dtmf,dtmf_len, obfusparam);
      stringify(dtmf_deobf,obfusparam->obfuslen);
      // Check that the user input preamble matches the obfuscated DTMF key
      if (strcasecmp((char*)dtmf_deobf,uinput)==0) {
         unsigned char * hint_deobf = deobfuscate(hint, hint_len, obfusparam);
         stringify(hint_deobf,obfusparam->obfuslen);
         funprint((char*)hint_deobf, 10000);
         free(hint_deobf);
      } else {
         funprint("Incorrect preamble, terminating", 30000);
         funprint(".........\n", 100000);
         return 0;
      }
      free(dtmf_deobf);

   } else if (secret_byte == 0x5a){
      unsigned char * unlock_deobf = deobfuscate(unlock, unlock_len, obfusparam);
      stringify(unlock_deobf,obfusparam->obfuslen);
      funprint((char*)unlock_deobf, 20000);
      free(unlock_deobf);

      obfusparam->xor_key = 0x23;
      obfusparam->scheme = 'x';
      obfusparam->padding = 'n';
      unsigned char * payload_deobf = deobfuscate(payload, payload_len, obfusparam);
      stringify(payload_deobf,obfusparam->obfuslen);
      write_output("layer2.txt", payload_deobf, obfusparam->obfuslen);
      funprint((char*)payload_deobf, 5000);
      free(payload_deobf);

   } else {
      funprint("Incorrect...\n", 100000);
      return 0;
   }

   getchar();
   return 0;
}
