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

#define ADC_RAW 16

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

   while (1) {
      printf("%d\t%d\t%d\t%d\n",fpgabase[ADC_RAW],fpgabase[ADC_RAW+1],fpgabase[ADC_RAW+2],fpgabase[ADC_RAW+3]);
      usleep(100000);
   }   

return 0; 

}
