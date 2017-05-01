#ifndef __ADCONV__
#define __ADCONV__

#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"

#ifdef __cplusplus
 extern "C" {
#endif

  #define SIZEBUFFERCH     32
  #define SIZEBUFFADC     SIZEBUFFERCH * 2
  
  struct CH{
    uint16_t buffer[SIZEBUFFERCH];
    float bufferfilter[SIZEBUFFERCH];
    uint16_t  hpf_lastsample;
    float  hpf_lastfilter;    
    
    float rms;
  };
  
   void init_adcConv(void);
   void start_adcConv(void);
   void WAIT_Conv_IS_DONE(void);
   
   void hpf_ch1(void);
   void hpf_ch2(void);

#ifdef __cplusplus
}
#endif


#endif 

