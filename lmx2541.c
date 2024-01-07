/********************************************************************
*  Mem-Map
*  8-17-2016
*  Tony Caracappa
*  This program reads the shared memory space
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
    int regAddr, regVal, command, rdbk;

    /* Open /dev/mem for writing to FPGA register */
    fd = open("/dev/mem",O_RDWR|O_SYNC);
    if (fd < 0)  {
      printf("Can't open /dev/mem\n");
      return 1;
   }

   fpgabase = (unsigned int *) mmap(0,255,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x43C00000);

   if (fpgabase == NULL) {
      printf("Can't mmap\n");
      return 1;
   }

   //register R7 : Resets all registers
   fpgabase[4] = 0x00000017;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R13 
   fpgabase[4] = 0x0000008d;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R12 : external VCO
   fpgabase[4] = 0x0000001c;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R9  : program as shown
   fpgabase[4] = 0x28001409;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R8 : 
   fpgabase[4] = 0x0111ce58;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R6
   fpgabase[4] = 0x001f3336;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R5
   fpgabase[4] = 0xA0000005;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R4
   fpgabase[4] = 0x88084754;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R3
   fpgabase[4] = 0x00387f03;
   fpgabase[5] = 0x00000001;
   fpgabase[5] = 0x00000000;
   usleep(100000);

   // R2
   fpgabase[4] = 0x04000002;
   fpgabase[3] = 0x40000000;
   fpgabase[3] = 0x00000000;
   usleep(100000);

   // R1
   fpgabase[4] = 0x000009b1;
   fpgabase[3] = 0x40000000;
   fpgabase[3] = 0x00000000;
   usleep(100000);

   // R0
   fpgabase[4] = 0x0001a4f0;
   fpgabase[3] = 0x40000000;
   fpgabase[3] = 0x00000000;
   usleep(100000);

   return 0; 

}
