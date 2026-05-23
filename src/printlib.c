#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

void funprint(char* s_ptr, int delay){
   while(*s_ptr != 0x00) {
      putchar(*s_ptr++);
      fflush(stdout);
      usleep(delay);
   }
}

// Take a pointer to binary blob and reallocate it to add a null terminator for printf memory safety
char* stringify (unsigned char* ptr, size_t size) {
   ptr = realloc((char*)ptr, size+1);
   ptr[size]=0x00;
   return (char*)ptr;
}
