/********************************************************************
*  Set Attenuator 
*  3-15-15
*  
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
    unsigned int regAddr, regVal, command, rdbk;

    if (argc < 2) {
       printf("Usage: %s value\n",argv[0]);
       exit(1);
    }
    
    regVal  = strtoul(argv[1],NULL,0); //atoi(argv[3]);
    if (regVal > 31) {
       printf("Max Atten = 31dB\n");
       exit(1);
    }
    printf("Setting Attenuator to  %d dB\n",regVal);
    regVal = regVal * 4;   //each bit is 0.25dB


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
   printf("val: %x\n",(0x80000000 | regVal));
   fpgabase[3] = 0x80000000 | regVal;     
   usleep(1000); 
   fpgabase[3] = 0; 

 
   return 0; 

}
