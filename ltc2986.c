/********************************************************************
*  LTC2986 Temperature Readout Chip 
*  8-3-2020
*  
*  This program reads the thermistor temperatures from the LTC2986
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define LTCREG 20

int main()
{
    unsigned int fpgaAddr, fpgaData;
    volatile unsigned int *fpgabase;
    int fd;
    int dat1, dat2, dat3, stat;
    float temp1, temp2, temp3, temp4;

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

// First set up the 10KOhm sense resistor on Channel 2....
   fpgabase[LTCREG] = 0x020204e8;
   usleep(1000);
   fpgabase[LTCREG] = 0x0202059c;
   usleep(1000);
   fpgabase[LTCREG] = 0x02020640;
   usleep(1000);
   fpgabase[LTCREG] = 0x02020700;
   usleep(1000);

// Set up the first thermistor on channel 4...
   fpgabase[LTCREG] = 0x02020cb0;
   usleep(1000);
   fpgabase[LTCREG] = 0x02020d8c;
   usleep(1000);
   fpgabase[LTCREG] = 0x02020e00;
   usleep(1000);
   fpgabase[LTCREG] = 0x02020f00;
   usleep(1000);

// Set up the second thermistor on channel 6...
   fpgabase[LTCREG] = 0x020214b0;
   usleep(1000);
   fpgabase[LTCREG] = 0x0202158c;
   usleep(1000);
   fpgabase[LTCREG] = 0x02021600;
   usleep(1000);
   fpgabase[LTCREG] = 0x02021700;
   usleep(1000);

// Set up the third thermistor on channel 8...
   fpgabase[LTCREG] = 0x02021cb0;
   usleep(1000);
   fpgabase[LTCREG] = 0x02021d8c;
   usleep(1000);
   fpgabase[LTCREG] = 0x02021e00;
   usleep(1000);
   fpgabase[LTCREG] = 0x02021f00;
   usleep(1000);

// Set up the fourth thermistor on channel 10...
   fpgabase[LTCREG] = 0x020224b0;
   usleep(1000);
   fpgabase[LTCREG] = 0x0202258c;
   usleep(1000);
   fpgabase[LTCREG] = 0x02022600;
   usleep(1000);
   fpgabase[LTCREG] = 0x02022700;
   usleep(1000);


// Now read temperatures in a loop:
   while(1==1){
      // Read the first thermistor on CH4:
      fpgabase[LTCREG] = 0x02000084;
      usleep(170000);
      fpgabase[LTCREG] = 0x03001f00;
      usleep(1000);
      dat1 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03001e00;
      usleep(1000);
      dat2 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03001d00;
      usleep(1000);
      dat3 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03001c00;
      usleep(1000);
      stat = fpgabase[LTCREG];
      temp1 = (float)(dat1 + dat2*256 + dat3*65536)/1024.0;

      // Read the second thermistor on CH6:
      fpgabase[LTCREG] = 0x02000086;
      usleep(170000);
      fpgabase[LTCREG] = 0x03002700;
      usleep(1000);
      dat1 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03002600;
      usleep(1000);
      dat2 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03002500;
      usleep(1000);
      dat3 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03002400;
      usleep(1000);
      stat = fpgabase[LTCREG];
      temp2 = (float)(dat1 + dat2*256 + dat3*65536)/1024.0;

      // Read the third thermistor on CH8:
      fpgabase[LTCREG] = 0x02000088;
      usleep(170000);
      fpgabase[LTCREG] = 0x03002f00;
      usleep(1000);
      dat1 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03002e00;
      usleep(1000);
      dat2 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03002d00;
      usleep(1000);
      dat3 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03002c00;
      usleep(1000);
      stat = fpgabase[LTCREG];
      temp3 = (float)(dat1 + dat2*256 + dat3*65536)/1024.0;

      // Read the second thermistor on CH6:
      fpgabase[LTCREG] = 0x0200008A;
      usleep(170000);
      fpgabase[LTCREG] = 0x03003700;
      usleep(1000);
      dat1 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03003600;
      usleep(1000);
      dat2 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03003500;
      usleep(1000);
      dat3 = fpgabase[LTCREG];
      fpgabase[LTCREG] = 0x03003400;
      usleep(1000);
      stat = fpgabase[LTCREG];
      temp4 = (float)(dat1 + dat2*256 + dat3*65536)/1024.0;

      printf("Temperatures = %5.2f  %5.2f  %5.2f  %5.2f\n",temp1,temp2,temp3,temp4);
   }

 
   return 0; 

}
