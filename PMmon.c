/********************************************************************
*  Power Manager Readbacks for ZYNQ-based RF BPM
*  6-27-2017
*  By: Anthony Caracappa
*  This subroutine is called by an IOC for the 1 Hz Readbacks... 
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <math.h>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <errno.h>
#include <i2c/smbus.h>


float L16_to_float(int input_val)
{
    // fixed exponent of -13 on LTC2977
    int exponent = -13;
    int mantissa = input_val;
    // compute value as mantissa * 2^(exponent)
    return mantissa * pow(2,exponent);
}

float L11_to_float(int input_val)
{
    // extract exponent as MS 5 bits
    int exponent = input_val >> 11;
    // extract mantissa as LS 11 bits
    int mantissa = input_val & 0x7ff;
    // sign extend exponent from 5 to 8 bits
    if( exponent > 0x0F ) exponent = (exponent|0xE0)-256; 
    // ints are 32 bits so using subtraction to adjust for negative numbers
    // sign extend mantissa from 11 to 16 bits
    if( mantissa > 0x03FF ) mantissa = (mantissa|0xF800)-65536;
    // compute value as mantissa * 2^(exponent)
    return mantissa * pow(2,exponent);
}

static int PowMan()
{
    volatile unsigned int *fpgabase;
    int fd;

    /* Open /dev/mem for writing to FPGA register */
    fd = open("/dev/mem",O_RDWR|O_SYNC);
    if (fd < 0)  {
      printf("PowMan.c: Can't open /dev/mem\n");
      return 1;
   }

   fpgabase = (unsigned int *) mmap(0,128,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x43C00000);


   if (fpgabase == NULL) {
      printf("PowMan.c: Can't mmap\n");
      fclose(fd);
      return 1;
   }
   
   int file;
   char filename[20];
  
   snprintf(filename, 19, "/dev/i2c-0");
   file = open(filename, O_RDWR);
   if (file < 0){
      /* ERROR HANDLING; you can check errno to see what went wrong */
      exit(1);
   }
   int addr = 0x5b; /* The I2C address */

   if (ioctl(file, I2C_SLAVE, addr) < 0){
      /* ERROR HANDLING; you can check errno to see what went wrong */
      exit(1);
   }
   
   int res;
   short stats;
   float temp;

   res = i2c_smbus_read_word_data(file, 0x8d);
   temp = L11_to_float(res);
   //*(float *)precord->vala = temp;
   printf("Val = %f\n",temp);

   res = i2c_smbus_read_word_data(file, 0x88);
   temp = L11_to_float(res);
   //*(float *)precord->valb = temp;
   printf("Val = %f\n",temp);

   // Page 0
   i2c_smbus_write_byte_data(file, 0x00, 0 );
   res = i2c_smbus_read_word_data(file, 0x8b);
   stats = i2c_smbus_read_word_data(file, 0x79);
   if(stats<0) stats = i2c_smbus_read_word_data(file, 0x79);
   temp = L16_to_float(res);
   if(temp>7.9){
      usleep(20000);
      res = i2c_smbus_read_word_data(file, 0x8b);
      temp = L16_to_float(res);
   }
   //*(float *)precord->vald = temp;
   //*(int *)precord->vall = stats;
   printf("Val = %f\n",temp);



   // Page 1
   i2c_smbus_write_byte_data(file, 0x00, 1 );
   res = i2c_smbus_read_word_data(file, 0x8b);
   stats = i2c_smbus_read_word_data(file, 0x79);
   if(stats<0) stats = i2c_smbus_read_word_data(file, 0x79);
   temp = L11_to_float(res);
   if(temp<0){
      usleep(20000);
      res = i2c_smbus_read_word_data(file, 0x8b);
      temp = L11_to_float(res);
   }
   //*(float *)precord->vale = temp*100.0;
   //*(int *)precord->valm = stats;
   printf("Val = %f\n",temp);



   // Page 2
   i2c_smbus_write_byte_data(file, 0x00, 2 );
   res = i2c_smbus_read_word_data(file, 0x8b);
   stats = i2c_smbus_read_word_data(file, 0x79);
   if(stats<0) stats = i2c_smbus_read_word_data(file, 0x79);
   temp = L16_to_float(res);
   if(temp>7.9){
      usleep(20000);
      res = i2c_smbus_read_word_data(file, 0x8b);
      temp = L16_to_float(res);
   }
   //*(float *)precord->valf = temp;
   //*(int *)precord->valn = stats;
   printf("Val = %f\n",temp);




   i2c_smbus_write_byte_data(file, 0x00, 3 );
   res = i2c_smbus_read_word_data(file, 0x8b);
   stats = i2c_smbus_read_word_data(file, 0x79);
   if(stats<0) stats = i2c_smbus_read_word_data(file, 0x79);
   temp = L16_to_float(res);
   if(temp>7.9){
      usleep(20000);
      res = i2c_smbus_read_word_data(file, 0x8b);
      temp = L16_to_float(res);
   }
   //*(float *)precord->valg = temp;
   //*(int *)precord->valo = stats;
   printf("Val = %f\n",temp);




   i2c_smbus_write_byte_data(file, 0x00, 4 );
   res = i2c_smbus_read_word_data(file, 0x8b);
   stats = i2c_smbus_read_word_data(file, 0x79);
   if(stats<0) stats = i2c_smbus_read_word_data(file, 0x79);
   temp = L16_to_float(res);
   if(temp>7.9){
      usleep(20000);
      res = i2c_smbus_read_word_data(file, 0x8b);
      temp = L16_to_float(res);
   }
   //*(float *)precord->valh = temp;
   //*(int *)precord->valp = stats;
   printf("Val = %f\n",temp);


   
   i2c_smbus_write_byte_data(file, 0x00, 5 );
   res = i2c_smbus_read_word_data(file, 0x8b);
   stats = i2c_smbus_read_word_data(file, 0x79);
   if(stats<0) stats = i2c_smbus_read_word_data(file, 0x79);
   temp = L16_to_float(res);
   if(temp>7.9){
      usleep(20000);
      res = i2c_smbus_read_word_data(file, 0x8b);
      temp = L16_to_float(res);
   }
   //*(float *)precord->vali = temp;
   //*(int *)precord->valq = stats;
   printf("Val = %f\n",temp);


   i2c_smbus_write_byte_data(file, 0x00, 6 );
   res = i2c_smbus_read_word_data(file, 0x8b);
   stats = i2c_smbus_read_word_data(file, 0x79);
   if(stats<0) stats = i2c_smbus_read_word_data(file, 0x79);
   temp = L16_to_float(res);
   if(temp>7.9){
      usleep(20000);
      res = i2c_smbus_read_word_data(file, 0x8b);
      temp = L16_to_float(res);
   }
   //*(float *)precord->valj = temp;
   //*(int *)precord->valr = stats;
   printf("Val = %f\n",temp);




   i2c_smbus_write_byte_data(file, 0x00, 7 );
   res = i2c_smbus_read_word_data(file, 0x8b);
   stats = i2c_smbus_read_word_data(file, 0x79);
   if(stats<0) stats = i2c_smbus_read_word_data(file, 0x79);
   temp = L11_to_float(res);
   if(temp<0){
      usleep(20000);
      res = i2c_smbus_read_word_data(file, 0x8b);
      temp = L11_to_float(res);
   }
   //*(float *)precord->valc = temp*100.0;
   //*(int *)precord->valk = stats;
   printf("Val = %f\n",temp);



/*
   if(*(int *)precord->a > 0){
      *(int *)precord->s = 0;
      i2c_smbus_write_byte_data(file, 0x00, 0xff ); // Global page
      i2c_smbus_write_byte(file, 0x03); // Clear Faults
      i2c_smbus_write_byte_data(file, 0x00, 0 ); // page 0
      res =  i2c_smbus_read_word_data(file, 0x79); //read fault for page 0
   }
*/
   if(munmap(fpgabase, 128) == -1){
      printf("PowMan: Error closing mmap\n");
   }
   close(fd);
   close(file);
   return 0;
}

