#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h>

#define TEMPREG 0x70/4 
#define SACNTREG 0x1A8/4 

#define kx 0.01
#define ky 0.01


volatile unsigned int *fpgabase;  //mmap'd fpga registers


/*******************************************
* Read ADC
*
*
*********************************************/
int read_temps(int *tempbuf)
{

    int i, val;

    for (i=0;i<=3;i++) {
        *tempbuf++ = fpgabase[TEMPREG+i];  
        
    } 
}


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
void main( int argc, char *argv[] )
{
   int i;
   int dacVal, dacAddr;
   float val;
   int tempraw[8];
   float temp[8];
   int sacnt = 0;
   int prev_sacnt = 0;
   float xpos, ypos, sum;
 
   mmap_fpga();

   printf("ChA\tChB\tChC\tChD\n");      

 
   for (i=0;i<40000;i++) {
      sacnt = fpgabase[SACNTREG];
      if (sacnt != prev_sacnt) {
          prev_sacnt = sacnt;
          read_temps(tempraw);


          for (i=0;i<=3;i++) {
            if (tempraw[i] >= 0x8000)
               tempraw[i] = tempraw[i] | 0xFFFF0000;         
            temp[i] = tempraw[i] * 0.0078125;
          } 

          printf("%d  ",sacnt);
          for (i=0;i<=3;i++)
            // printf("%x    ",tempraw[i]);
             printf("%8.4f  ",temp[i]);
          printf("\n");

      }
      usleep(1000);

   } 

}
