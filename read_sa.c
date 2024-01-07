#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>


#define SAREG 0x1B8/4 
#define SACNTREG 0x1A8/4 

#define kx 0.01
#define ky 0.01


volatile unsigned int *fpgabase;  //mmap'd fpga registers


/*******************************************
* Read ADC
*
*
*********************************************/
int read_sa(int *sabuf)
{

    int i, val;

    for (i=0;i<=7;i++) {
        *sabuf++ = fpgabase[SAREG+i];  
        
        //printf("i=%d\tval=%d\n",i,val);
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
main( int argc, char *argv[] )
{
   int i;
   int dacVal, dacAddr;
   float val;
   int saraw[8];
   float sa[8];
   int sacnt = 0;
   int prev_sacnt = 0;
   float xpos, ypos, sum;
 
   mmap_fpga();

   printf("ChA\tChB\tChC\tChD\n");      

 
   for (i=0;i<40000;i++) {
      sacnt = fpgabase[SACNTREG];
      if (sacnt != prev_sacnt) {
          prev_sacnt = sacnt;
          read_sa(saraw);
//          for (i=0;i<=7;i++)
//             printf("%10d  ",saraw[i]);
//          printf("\n");


          for (i=0;i<=7;i++)          
            sa[i] = saraw[i] / pow(2,31);
          sa[4] = sa[4] * kx * 1e6; //position in um
          sa[5] = sa[5] * ky * 1e6; //position in um
          sum = sa[0] + sa[1] + sa[2] + sa[3];
          xpos = kx *(((sa[0] + sa[3]) - (sa[1] + sa[2])) / sum) * 1e6;
          ypos = ky *(((sa[0] + sa[1]) - (sa[2] + sa[3])) / sum) * 1e6;
 

          printf("%d  ",sacnt);
          for (i=0;i<=7;i++)
             printf("%10.7f  ",sa[i]);
          printf("   \t%10.7f  %10.7f  %10.7f",xpos,ypos,sum);
          printf("\n");

      }
      usleep(1000);

   } 

}
