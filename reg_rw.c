/********************************************************************
*  Reg Poke 
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
    unsigned int regAddr, regVal, command, rdbk;

    if (argc < 3) {
       printf("Usage: %s [R W] addr value\n",argv[0]);
       exit(1);
    }
    
    if (strcmp(argv[1],"W") == 0) {
       command = 0;
       regAddr = strtol(argv[2],NULL,0); //atoi(argv[2]);
       regVal  = strtoul(argv[3],NULL,0); //atoi(argv[3]);
       printf("Writing Register %d to %d  (0x%x)\n",regAddr,regVal,regVal);
       }

    else if (strcmp(argv[1], "R") == 0) {
       command = 1;
       regAddr = strtol(argv[2],NULL,0); //atoi(argv[2]);
       printf("Reading Register %d\n",regAddr);
    }
    else
        printf("Usage: %s [R W] reg value\n",argv[0]);
    


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

   if (command == 1) {
       rdbk = fpgabase[regAddr]; 
       printf("Value = %d    0x%x\n",rdbk,rdbk);
   }
   else {
       fpgabase[regAddr] = regVal;     
       printf("Value Written...\n");
   }

 
   return 0; 

}
