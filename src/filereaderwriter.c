#include<stdlib.h>
#include<stdio.h>
#include"filereaderwriter.h"
#include"printlib.h"

size_t read_data(char * filename, unsigned char** buffer)
{
   size_t file_len;

   FILE * fp = fopen(filename, "r");
   if (fp == NULL)
   {
      fprintf(stderr, "File error\n");
      return -1;
   }

   // Forward pointer to end of file
   fseek(fp, 0, SEEK_END);
   file_len = ftell(fp);
   // Rewind file pointer
   fseek(fp, 0, SEEK_SET);

   *buffer = malloc (sizeof(unsigned char)*file_len);
   size_t ret_code = fread(*buffer, sizeof(unsigned char), file_len, fp);
   if(ret_code == file_len) {
       DEBUG_PRINT("File read successfully\n");
   } else { // error handling
         printf("Error reading %s\n", filename);
      if (feof(fp))
         printf("Error reading %s: unexpected end of file\n", filename);
      else if (ferror(fp)) {
         perror("Error reading file");
      }
   }

   return file_len;
}

int write_output(char* filename, unsigned char* buffer, size_t file_len){
   FILE * fp = fopen(filename, "wb");
   if (fp == NULL)
   {
      fprintf(stderr, "File error\n");
      return -1;
   }
   fwrite(buffer, sizeof(unsigned char), file_len, fp);
   fclose(fp);
   DEBUG_PRINT("Wrote file!\n");
   return 0;
}
