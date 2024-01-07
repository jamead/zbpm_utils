#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>


#define SAREG 110 
#define SACNTREG 106 
#define CHA_GAIN 38
#define CHB_GAIN 39
#define CHC_GAIN 40
#define CHD_GAIN 41



#define kx 0.01
#define ky 0.01


volatile unsigned int *fpgabase;  //mmap'd fpga registers



/*******************************************
* mmap fpga register space
* returns pointer fpgabase
*
********************************************/
void mmap_fpga()
{
   int fd;


   fd = open("/dev/mem",O_RDWR|O_SYNC);
   if (fd < 0) {
      printf("Can't open /dev/mem\n");
      exit(1);
   }

   fpgabase = (unsigned int *) mmap(0,255,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x43C00000);

   if (fpgabase == NULL) {
      printf("Can't map FPGA space\n");
      exit(1);
   }

}



/*******************************************
* Main 
* 
*
*
*******************************************/
main( int argc, char *argv[] )
{
   int i;
   float saraw[4];
   float gain[4];
   int gain_reg[4];
   float max_val; 

   mmap_fpga();


   //clear any gains that may already be present
   fpgabase[CHA_GAIN] = 0x7FFF;
   fpgabase[CHB_GAIN] = 0x7FFF;
   fpgabase[CHC_GAIN] = 0x7FFF;
   fpgabase[CHD_GAIN] = 0x7FFF;
    



}
