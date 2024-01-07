/*   Read ADC FIFO
 *
 */


#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


#define FIFODATAREG 8    //16 
#define FIFOWDCNTREG 9   //17 
#define FIFOCNTRLREG 10  //18 
#define FIFOSTREAMENB 11  //19 


int main(int argc, char *argv[])
{
    int dev;
    char filename[40], buf[10];
    int addr = 0b01001000;        // The I2C address of the ADC
    int i,j;
    float rdback, voltage;
    int boardNum, channelNum;
    unsigned int fpgaAddr, fpgaData;
    volatile unsigned int *fpgabase;
    int fd;
    float elapsedTime, runTime;
    int fadatabuf[2000000];
    int wordCnt, faEnabled, wordsRead, regVal;
    int num_samples;
    int error=0;
    int samp_cnt = 0;
    int fadiv;
    float chx, chy;

    /* Open /dev/mem for writing to FPGA register */
    fd = open("/dev/mem",O_RDWR|O_SYNC);
    if (fd < 0)  {
      printf("Can't open /dev/mem\n");
      return 1;
    }
    //printf("Opened /dev/mem\r\n");

    fpgabase = (unsigned int *) mmap(0,255,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x43C20000);
    if (fpgabase == NULL) {
      printf("Can't mmap\n");
      return 1;
    }
    printf("Resetting FIFO...\n");
    fpgabase[FIFOCNTRLREG] = 0x1;
    usleep(100);
    fpgabase[FIFOCNTRLREG] = 0x0;
    usleep(100); 
    printf("Words in FIFO = %d\n",fpgabase[FIFOWDCNTREG]); 
    printf("Starting TbT FIFO write burst (8K samples)\n");
    fpgabase[FIFOSTREAMENB] = 0;
    fpgabase[FIFOSTREAMENB] = 1;
    fpgabase[FIFOSTREAMENB] = 0; 
    printf("Waiting for TbT Data to Start...\n");
    while (fpgabase[FIFOWDCNTREG] == 0)
         usleep(10000);
    printf("Running...\n");
    usleep(500000);
    wordCnt = fpgabase[FIFOWDCNTREG];
    wordsRead = 0;
    printf("FIFO Word Count: %d\n",wordCnt);

    while (wordCnt != 0) {
       wordCnt = fpgabase[FIFOWDCNTREG];
       regVal = fpgabase[FIFODATAREG]; 
       fadatabuf[wordsRead] = regVal;
        wordsRead++;
       }
 
    printf("Run Complete... ");
    printf("Words Read = %d\n",wordsRead);
    printf("Remaining FA Word Count : %d\n",fpgabase[FIFOWDCNTREG]);
    printf("Results...\n");
    samp_cnt = 0;

    for (i=3;i<200;i=i+2) {
	 chx = (float)fadatabuf[i] / 1000.0;
	 chy = (float)fadatabuf[i+1] / 1000.0;
         printf("%8.3f\t%8.3f\n",chx,chy);
         samp_cnt++;
       }
    printf("Header: %8x\n",fadatabuf[0]);
    printf("Time Stamp: %8d %8d\n", fadatabuf[1],fadatabuf[2]);

}
