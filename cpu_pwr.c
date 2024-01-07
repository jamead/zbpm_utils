/********************************************************************
*  CPU Readbacks for ZYNQ-based RF BPM
*  8-29-2023
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

void cpu_pwr(char *msg)
{
   FILE *fp;
   char resp[50];
   float offset, raw, scale, temp, vccint, vccaux, vccbram, vccpint, vccpaux, vccoddr;
   float vrefp, vrefn, sampFreq;

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_temp0_offset", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&offset);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_temp0_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_temp0_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   temp = scale*(raw + offset)/1000.0;
   //printf("Die Temp = %f\n",temp);

   memcpy(msg,&temp,sizeof(int));
   
   /*
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage0_vccint_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage0_vccint_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   vccint = scale*raw/1000.0;
   printf("Vccint = %f\n",vccint);

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage1_vccaux_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage1_vccaux_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   vccaux = scale*raw/1000.0;
   printf("Vccaux = %f\n",vccaux);

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage2_vccbram_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage2_vccbram_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   vccbram = scale*raw/1000.0;
   printf("Vccbram = %f\n",vccbram);

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage3_vccpint_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage3_vccpint_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   vccpint = scale*raw/1000.0;
   printf("Vccpint = %f\n",vccpint);

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage4_vccpaux_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage4_vccpaux_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   vccpaux = scale*raw/1000.0;
   printf("Vccpaux = %f\n",vccpaux);

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage5_vccoddr_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage5_vccoddr_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   vccoddr = scale*raw/1000.0;
   printf("Vccoddr = %f\n",vccoddr);

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage6_vrefp_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage6_vrefp_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   vrefp = scale*raw/1000.0;
   printf("Vrefp = %f\n",vrefp);

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage7_vrefn_raw", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/in_voltage7_vrefn_scale", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&scale);
   pclose(fp);
   vrefn = scale*raw/1000.0;
   printf("Vrefn = %f\n",vrefn);

   fp = popen("cat /sys/bus/platform/drivers/xadc/f8007100.adc/iio\\:device0/sampling_frequency", "r");
   sscanf(fgets(resp, sizeof(resp), fp),"%f",&raw);
   pclose(fp);
   sampFreq = 0.001*raw;
   printf("Sample Freq = %f\n",sampFreq);

*/

}

