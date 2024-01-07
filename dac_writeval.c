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

#define DACREG 24

int main( int argc, char *argv[] )
{
    volatile unsigned int *fpgabase;
    int fd;
    int i;
    int dacnum, dacch, dacval,dacreg;


    if (argc < 4) {
       printf("Usage: %s dac_num(0,1) dac_ch(0,1,2,3) value in mV (0 to 10800)\n",argv[0]);
       exit(1);
       }

    dacnum = atoi(argv[1]);
    dacch = atoi(argv[2]);
// Next line changed to allow DAC value to be in units of mV
    dacval  = (int) (65535.0*(float)atoi(argv[3])/10800.0);
    printf("Writing DAC %d Chan %d to %d\n",dacnum,dacch,dacval);




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

   fpgabase[DACREG] = 0x10000F + 0x01000000*dacnum;  //turn on all DAC's
   usleep(10000);
   fpgabase[DACREG] = 0x0C0002 + 0x01000000*dacnum;  //set range to 0v to +10.8v all 4 channels(Unipolar for Heaters)
   usleep(10000);

   dacreg = dacval + 65536*dacch + 0x01000000*dacnum;
   printf("dac reg: %x\n",dacreg);
   fpgabase[DACREG] = dacreg;
   
  return 0; 

}
