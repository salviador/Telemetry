#ifndef __SERIALCHART__
#define __SERIALCHART__

#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"
#include "adconv.h"

#ifdef __cplusplus
 extern "C" {
#endif

   void init_serialChart(void);
   void serialChart_write(uint8_t *data);
   uint8_t serialChart_wait_receiveByte(uint8_t *data);
   void serialChart_sendJsonTelemetry(struct CH channel1, struct CH channel2, uint16_t ch1size);

   
   

#ifdef __cplusplus
}
#endif


#endif 

