#include <sys/types.h>  
#include <sys/stat.h>  
#include <io.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>
#include <iostream>  

using namespace std;

void chmod(char * filename, int mask) {  
   // Check for failure   
   if( _chmod( filename, mask ) == -1 ) {  
       // Determine cause of failure and report.   
      switch (errno) {  
         case EINVAL:  
            fprintf(stderr, "Invalid parameter to chmod.\n");  
            break;  
         case ENOENT:  
            fprintf(stderr, "File %s not found\n", filename);  
            break;  
         default:  
            fprintf(stderr, "Unexpected error in chmod.\n");  
       }  
   }  else {  
        if (mask == _S_IREAD)  
            printf( "Mode set to read-only\n" );  
        else if (mask & _S_IWRITE)  
            printf( "Mode set to read/write\n" );  
   }  
   fflush(stderr);  
}  

int main(int argsc, char* argsv[]) {
   chmod(argsv[1], strtol(argsv[2], 0, 8));  
} 