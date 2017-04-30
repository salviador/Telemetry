#ifndef __ADCONV__
#define __ADCONV__

#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"

#ifdef __cplusplus
 extern "C" {
#endif

  #define SIZEBUFFERCH1      16
  
  struct CH1{
    uint16_t buffer[SIZEBUFFERCH1];
    float bufferfilter[SIZEBUFFERCH1];
    uint16_t  hpf_lastsample;
    float  hpf_lastfilter;    
  };
   
   void init_adcConv(void);
   void start_adcConv(void);

   void hpf_ch1(void);
   
#ifdef __cplusplus
}
#endif


#endif 

