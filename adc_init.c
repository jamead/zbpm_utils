/********************************************************************
*  ADC INIT 
*  3-15-15
*  
*  This program resets and aligns the AD9653 serial data 
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>


#define ADC_SPI 13
#define BITSLIP 6
#define ADC_RAW 16

volatile unsigned int *fpgabase;
int adc_data[4];




 
void read_rawadc(int *adc_data)
{
   int i; 

   for (i=0;i<=3;i++)
      adc_data[i] = fpgabase[ADC_RAW+i];
    
}

void bitslip(lanenum)
{
      fpgabase[BITSLIP] = lanenum; 
}


void adcspi_write(addr,data)
{
   fpgabase[ADC_SPI] = addr<<8 | data;
}


void align_adc()
{
    int aligned;
    
    //ADC 0
    aligned = 0;
    while (aligned == 0) {
       bitslip(0x1);
       usleep(1);
       bitslip(0x0);
       usleep(1);
       read_rawadc(adc_data);
       printf("ADCval=%x Bitslip ADC0 lane 0\n",(adc_data[0] & 0x00FF));
       if      ((adc_data[0] & 0x00FF) == 0x55)  aligned = 1;
       else if ((adc_data[0] & 0x00FF) == 0xAA)  aligned = 1;
    }

    aligned = 0;
    while (aligned == 0) {
       bitslip(0x2);
       usleep(1);
       bitslip(0x0);
       usleep(1);
       read_rawadc(adc_data);       
       printf("ADCval=%x Bitslip ADC0 lane 1\n",(adc_data[0] & 0xFF00));
       if      ((adc_data[0] & 0xFF00) == 0x5500)  aligned = 1;
       else if ((adc_data[0] & 0xFF00) == 0xAA00)  aligned = 1;
    }

    //ADC1
    aligned = 0;
    while (aligned == 0) {
       bitslip(0x4);
       usleep(1);
       bitslip(0x0);
       usleep(1);
       read_rawadc(adc_data);
       printf("ADCval=%x Bitslip ADC1 lane 0\n",(adc_data[1] & 0x00FF));
       if      ((adc_data[1] & 0x00FF) == 0x55)  aligned = 1;
       else if ((adc_data[1] & 0x00FF) == 0xAA)  aligned = 1;
    }

    aligned = 0;
    while (aligned == 0) {
       bitslip(0x8);
       usleep(1);
       bitslip(0x0);
       usleep(1);
       read_rawadc(adc_data);       
       printf("ADCval=%x Bitslip ADC1 lane 1\n",(adc_data[1] & 0xFF00));
       if      ((adc_data[1] & 0xFF00) == 0x5500)  aligned = 1;
       else if ((adc_data[1] & 0xFF00) == 0xAA00)  aligned = 1;
    }
    
    //ADC2
    aligned = 0;
    while (aligned == 0) {
       bitslip(0x10);
       usleep(1);
       bitslip(0x0);
       usleep(1);
       read_rawadc(adc_data);
       printf("ADCval=%x Bitslip ADC2 lane 0\n",(adc_data[2] & 0x00FF));
       if      ((adc_data[2] & 0x00FF) == 0x55)  aligned = 1;
       else if ((adc_data[2] & 0x00FF) == 0xAA)  aligned = 1;
    }

    aligned = 0;
    while (aligned == 0) {
       bitslip(0x20);
       usleep(1); 
       bitslip(0x0);
       usleep(1);
       read_rawadc(adc_data);       
       printf("ADCval=%x Bitslip ADC2 lane 1\n",(adc_data[2] & 0xFF00));
       if      ((adc_data[2] & 0xFF00) == 0x5500)  aligned = 1;
       else if ((adc_data[2] & 0xFF00) == 0xAA00)  aligned = 1;
    }

    //ADC3
    aligned = 0;
    while (aligned == 0) {
       bitslip(0x40);
       usleep(1);
       bitslip(0x0);
       usleep(1);
       read_rawadc(adc_data);
       printf("ADCval=%x Bitslip ADC3 lane 0\n",(adc_data[3] & 0x00FF));
       if      ((adc_data[3] & 0x00FF) == 0x55)  aligned = 1;
       else if ((adc_data[3] & 0x00FF) == 0xAA)  aligned = 1;
    }

    aligned = 0;
    while (aligned == 0) {
       bitslip(0x80);
       usleep(1);
       bitslip(0x0);
       usleep(1);
       read_rawadc(adc_data);       
       printf("ADCval=%x Bitslip ADC3 lane 1\n",(adc_data[3] & 0xFF00));
       if      ((adc_data[3] & 0xFF00) == 0x5500)  aligned = 1;
       else if ((adc_data[3] & 0xFF00) == 0xAA00)  aligned = 1;
    }
}



int main( int argc, char *argv[] )
{
    //volatile unsigned int *fpgabase;
    int fd;
    //int adc_data[4];
    int aligned;

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

   //Reset ADC
   printf("Resetting ADC...\n");
   adcspi_write(8,3);
   usleep(10000);
   adcspi_write(8,0);

   //Put ADC in test pattern mode
   printf("Putting ADC in test pattern mode...\n");
   adcspi_write(0xD,4);
   usleep(10000);

   align_adc(adc_data);

   read_rawadc(adc_data);
   printf("%x\t%x\t%x\t%x\n",adc_data[3],adc_data[2],adc_data[1],adc_data[3]); 

   //Put ADC in real adc  mode
   printf("Putting ADC in data  mode...\n");
   adcspi_write(0xD,0);
   usleep(10000);

   

   
   return 0; 

}
