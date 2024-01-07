/********************************************************************
*  FPGA Version
*  3-15-15
*  
*  This program reads the FPGA version
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>



int main( int argc, char *argv[] )
{
    unsigned int fpgaAddr, fpgaData;
    volatile unsigned int *fpgabase;
    int fd;

    /* Open /dev/mem for writing to FPGA register */
    fd = open("/dev/mem",O_RDWR|O_SYNC);
    if (fd < 0)  {
      printf("Can't open /dev/mem\n");
      return 1;
   }
   //printf("Opened /dev/mem\r\n");

   fpgabase = (unsigned int *) mmap(0,255,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x43C00000);


   if (fpgabase == NULL) {
      printf("Can't mmap\n");
      return 1;
   }
 
   printf("FPGA Version : %d   0x%x\n",fpgabase[100],fpgabase[100]);
    return 0; 

}
