/*
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
#include <math.h>

#define FIFODATAREG 0 
#define FIFOWDCNTREG 1
#define FIFORSTREG 2 
#define FIFOENBREG 3 
#define SOFTTRIGREG 80

int fadatabuf[2000000];
float fadata[2000000];
 
 
int main(int argc, char *argv[])
{
    int dev;
    char filename[40], buf[10];
    int addr = 0b01001000;        // The I2C address of the ADC
    int i,j;
    float rdback, voltage;
    int boardNum, channelNum;
    unsigned int fpgaAddr, fpgaData;
    volatile unsigned int *ioregbase,  *faregbase;
    int fd;
    float elapsedTime, runTime;
    int wordCnt, faEnabled, wordsRead, regVal;
    int num_samples;
    int error=0;
    int samp_cnt = 0;
 
    if (argc < 2) {
       printf("Usage: %s [samples] \n",argv[0]);
       exit(1);
    }
    num_samples = strtol(argv[1],NULL,0);
    num_samples = 8 * num_samples; //(8 32bit words are read out for each sample) 

    // Open /dev/mem for writing to FPGA register
    fd = open("/dev/mem",O_RDWR|O_SYNC);
    if (fd < 0)  {
      printf("Can't open /dev/mem\n");
      return 1;
    }
    //printf("Opened /dev/mem\r\n");

    ioregbase = (unsigned int *) mmap(0,255,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x43C00000);
    if (ioregbase == NULL) {
      printf("Can't mmap\n");
      return 1;
    }

    faregbase = (unsigned int *) mmap(0,255,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x43C10000);
    if (faregbase == NULL) {
      printf("Can't mmap\n");
      return 1;
     }


    printf("Resetting FIFO...\n");
    faregbase[FIFORSTREG] = 0x1;
    usleep(100);
    faregbase[FIFORSTREG] = 0x0;
    usleep(100); 
    printf("Words in FIFO = %d\n",faregbase[FIFOWDCNTREG]); 
    
    printf("Enabling FA Data to Start...\n");
    faregbase[FIFOENBREG] = 0x1; 
    

    printf("Soft Trigger...\n");
    ioregbase[SOFTTRIGREG] = 0x1; 
    ioregbase[SOFTTRIGREG] = 0x0; 
    printf("Running...\n");
    faEnabled = 1;
    wordCnt = faregbase[FIFOWDCNTREG];
    wordsRead = 0;
    printf("FIFO Word Count: %d\n",wordCnt);

    while (wordsRead < num_samples) {
       wordCnt = faregbase[FIFOWDCNTREG];
       if (wordCnt > 1200) {
          printf("FIFO Read Count=%d   Total Words Read = %d    Reading Another Block...\n",wordCnt,wordsRead);
          for (i=0;i<1200;i++) {
             regVal = faregbase[FIFODATAREG]; 
             fadatabuf[wordsRead] = regVal;
             wordsRead++;
          } 
       }
       usleep(10000); //sleep for 10ms (should be 100 10KHz samples)
       //faEnabled = fpgabase[SOFTTRIGREG] & 0x1; 
    }       
    printf("Stopping Run...");
    faregbase[FIFOENBREG] = 0x0;    

    printf("Run Complete... ");
    printf("Words Read = %d\n",wordsRead);
    printf("Remaining FA Word Count : %d\n",faregbase[FIFOWDCNTREG]);
    printf("Results...\n");

    //scale results
    i=3;
    while (i<wordsRead) {
       for (j=0;j<=3;j++) 
         fadata[i+j] = (float)fadatabuf[i+j] / pow(2,31);  //a,b,c,d
       fadata[i+4] = (float) fadatabuf[i+4] / 1000.0; // x scaled to um
       fadata[i+5] = (float) fadatabuf[i+5] / 1000.0; // y scaled to um
       fadata[i+7] = (float) fadatabuf[i+7] / pow(2,31);  //sum
       i=i+8; 
    }

    //header 0x80000000 
    printf("Header: %8x\n",fadatabuf[0]);      
    // time stamp 
    printf("Time Stamp: %8d  %8d\n",fadatabuf[1],fadatabuf[2]);    
    
    for (i=3;i<num_samples+3;i=i+8) {
         for (j=0;j<=7;j++) {
            printf("%8f   ",fadata[i+j]); // a,b,c,d, 
           }
         printf("\n");
     } 
     return 0;

}
