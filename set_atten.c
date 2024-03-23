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

#define RF 0
#define PT 1
#define PTATTEN_REG 3
#define RFATTEN_REG 6


int main( int argc, char *argv[] )
{
    unsigned int fpgaAddr, fpgaData;
    volatile unsigned int *fpgabase;
    int fd;
    unsigned int regAddr, regVal;
    int atten;

    if (argc < 3) {
       printf("Usage: %s [RF PT] value\n",argv[0]);
       exit(1);
    }
   
    if (strcmp(argv[1],"PT") == 0) {
	printf("Setting PT attenuator...\n");
	atten = PT;
    }
    else if (strcmp(argv[1],"RF") == 0) {
        printf("Setting RF attenuator...\n");
    	atten = RF;
    }
    else {
	printf("Invalid paramter\n");
	exit(1);
    }
	


    regVal  = strtoul(argv[2],NULL,0); //atoi(argv[3]);
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

   if (atten == RF)
      fpgabase[RFATTEN_REG] = regVal;     
   else
      fpgabase[PTATTEN_REG] = regVal;


 
   return 0; 

}
