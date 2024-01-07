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


#define FIFODATAREG 16 
#define FIFOWDCNTREG 17 
#define FIFOCNTRLREG 18 
#define FIFOSTREAMENB 19 


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
    int adcval_cha,adcval_chb,adcval_chc,adcval_chd;

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
    printf("Starting ADC FIFO write burt (8K samples)\n");
    fpgabase[FIFOSTREAMENB] = 1;
    fpgabase[FIFOSTREAMENB] = 0; 
    printf("Waiting for ADC Data to Start...\n");
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
    //for (i=0;i<=300;i+2)
//	printf("%2d:  %d\n",i,fadatabuf[i]);
    printf("ADC raw data\n");
    for (i=0;i<wordsRead;i=i+2) {
	 adcval_cha = (short int) ((fadatabuf[i] & 0xFFFF0000) >> 16);
	 adcval_chb = (short int) (fadatabuf[i] & 0xFFFF);    
         adcval_chc = (short int) ((fadatabuf[i+1] & 0xFFFF0000) >> 16);
	 adcval_chd = (short int) (fadatabuf[i+1] & 0xFFFF);    
         printf("%d\t%d\t%d\t%d\t%d\t\n",i,adcval_cha,adcval_chb,adcval_chc,adcval_chd);      
         samp_cnt++;
       }
    printf("Samples = %d\n",samp_cnt);
}
