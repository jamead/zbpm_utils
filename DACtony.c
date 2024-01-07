/********************************************************************
*  XBPM Setup Subroutine
*  3-12-2019
*  By: Anthony Caracappa
*  This subroutine is called by the xbpm IOC to set up channel params 
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>


#define DAC  24  //was 72,76 for electrometer
//#define LDAC 77  //was 73

main( int argc, char *argv[] )
{
    volatile unsigned int *fpgabase;
    float R, range;
    int DacA, DacB, DacC, DacD;
    int A,B,C,D;
    int fd, Mode;

    if (argc < 5) {
       printf("Usage: %s DACA DACB DACC DACD in mV (0 to 10800)\n",argv[0]);
       exit(1);
    }

    A = atoi(argv[1]);
    B = atoi(argv[2]);
    C = atoi(argv[3]);
    D = atoi(argv[4]);

    /* Open /dev/mem for writing to FPGA register */
    fd = open("/dev/mem",O_RDWR|O_SYNC);
    if (fd < 0)  {
      printf("DACSetup.c: Can't open /dev/mem\n");
      return 1;
    }

    fpgabase = (unsigned int *) mmap(0,255,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x43C00000);

    if (fpgabase == NULL) {
        printf("DACSetup.c: Can't mmap\n");
//        fclose(fd);
        return 1;
    }
    printf("Entering DAC Setup.....\n");

    range = 10800;

    if(A<0){
       A=0;
    }else if(A>range){
       A=range;
    }
    DacA = (int)(A*65535.0/range);
    if(B<0){
       B=0;
    }else if(B>range){
       B=range;
    }
    DacB = (int)(B*65535.0/range);
    if(C<0){
       C=0;
    }else if(C>range){
       C=range;
    }
    DacC = (int)(C*65535.0/range);
    if(D<0){
       D=0;
    }else if(D>range){
       D=range;
    }
    DacD = (int)(D*65535.0/range);

    fpgabase[DAC] = 1048591;
//    fpgabase[LDAC] = 0;
    usleep(500);
    fpgabase[DAC] = 0x0c0002;
    usleep(500);
    fpgabase[DAC] = DacA;
    usleep(500);
    fpgabase[DAC] = 65536 + DacB;
    usleep(500);
    fpgabase[DAC] = 131072 + DacC;
    usleep(500);
    fpgabase[DAC] = 196608 + DacD;
    usleep(500);
//   fpgabase[DAC] = 1900544;
//   fpgabase[LDAC] = 1;
//   fpgabase[LDAC] = 0;


    printf("Dac ADC codes= %d %d %d %d\n", DacA,DacB,DacC,DacD);


//    if(munmap(fpgabase,255) == -1){
//       printf("DACSetup.c: Error closing mmap\n");
//    }
//    close(fd);
    return 0;
}
