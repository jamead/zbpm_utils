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
* Read SA 
*
*
*********************************************/
void read_sa(float *sabuf)
{

    int i;

    for (i=0;i<=3;i++) {
        *sabuf++ = ( ((float)fpgabase[SAREG+i]) / pow(2,31));
        
        //printf("i=%d\tval=%d\n",i,val);
    } 
}


/*******************************************
* Return max value in array 
*
*
*********************************************/
float find_max(float *buf)
{

    int i;
    float max_val;

    max_val = 0;
    for (i=0;i<=3;i++) {
        if (buf[i] > max_val)
           max_val = buf[i]; 
    }       
    return max_val;
 
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
    
   for (i=0;i<10;i++) {
      read_sa(saraw); 
      printf("%5.5f   %5.5f   %5.5f   %5.5f\n",saraw[0],saraw[1],saraw[2],saraw[3]);
   } 


   read_sa(saraw);
   printf("%5.5f   %5.5f   %5.5f   %5.5f\n",saraw[0],saraw[1],saraw[2],saraw[3]);
   max_val = find_max(saraw);
   printf("Max Val = %5.5f\n\n",max_val);

   for (i=0;i<=3;i++) {
      gain[i] = max_val / saraw[i];
      gain_reg[i] = gain[i] * 0x7FFF;
      printf("gain[%d] = %5.5f  0x%x\n",i,gain[i],gain_reg[i]);
   }

   max_val = find_max(gain);
   printf("Max Val = %5.5f\n",max_val);

   //fractional data type: max value for gain is <1 (0x7FFF) 
   for (i=0;i<=3;i++) {
      gain[i] = (gain[i] / max_val);
      gain_reg[i] = gain[i] * 0x7FFF;
      printf("gain[%d] = %5.5f  0x%x\n",i,gain[i],gain_reg[i]);
   }


   fpgabase[CHA_GAIN] = gain_reg[0];
   fpgabase[CHB_GAIN] = gain_reg[1];
   fpgabase[CHC_GAIN] = gain_reg[2];
   fpgabase[CHD_GAIN] = gain_reg[3];





}
