#include "adconv.h"
#include <math.h>


#define   OUT_FREQ          10000                                // Input Frequency
#define   CAMPIONI_RES      32                                   // 16-Campioni
#define   CNT_FREQ          64000000                             // TIM3 counter clock (prescaled APB1)
#define   ADC_SAMPLIGTIME   ((CNT_FREQ)/((CAMPIONI_RES)*(OUT_FREQ))) // Autoreload reg value


uint16_t ADC_buffer[SIZEBUFFADC];


struct CH ch1;
struct CH ch2;
uint8_t ADC1_flag;

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
TIM_HandleTypeDef htim3;

void init_adcConv(void){
  GPIO_InitTypeDef GPIO_InitStruct;
  ADC_MultiModeTypeDef multimode;
  ADC_ChannelConfTypeDef sConfig;
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_ADC12_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    /**ADC1 GPIO Configuration    
    PA0     ------> ADC1_IN1
    PA1     ------> ADC1_IN2 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_NORMAL;
    hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&hdma_adc1);
    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);

    //ADC

      /**Common config 
      */
/*    
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    HAL_ADC_Init(&hadc1);
*/
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE; //ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = ENABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 2;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    
    hadc1.Init.NbrOfDiscConversion   = 1;                             /* Sequencer of regular group will convert ranks one by one, at each conversion trig */
      
    HAL_ADC_Init(&hadc1);
    
    /**Configure the ADC multi-mode 
    */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);

    /**Configure Regular Channel 
    */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 2;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);


    //Update PA1
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = 1;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    
    
    //TIMER 3
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = ADC_SAMPLIGTIME;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim3);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);      
    
    ADC1_flag = 0;

    ch1.hpf_lastfilter = 0;
    ch1.hpf_lastsample = 0;
    ch2.hpf_lastfilter = 0;
    ch2.hpf_lastsample = 0;
    
    HAL_TIM_Base_Start(&htim3);
   // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ch1.buffer, SIZEBUFFERCH1);    
    
    
  /* Run the ADC calibration in single-ended mode */  
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

 // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_buffer, SIZEBUFFADC);    
    
}

void start_adcConv(void){
    ADC1_flag = 0;
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_buffer, SIZEBUFFADC);    
}

void WAIT_Conv_IS_DONE(void){
  uint16_t i;
  uint8_t ch1c,ch2c;
  
  while(ADC1_flag==0){}
  
  //Restore buffer
  ch1c = 1;
  ch2c = 0;
  for(i=0;i<SIZEBUFFADC;i++){
    ch1.buffer[i] = ADC_buffer[ch1c];
    ch2.buffer[i] = ADC_buffer[ch2c];
      ch1c = ch1c + 2;
      ch2c = ch2c + 2;    
  }
}




//https://developer.mbed.org/forum/helloworld/topic/3051/
//https://learn.openenergymonitor.org/electricity-monitoring/ctac/digital-filters-for-offset-removal?redirected=true
void hpf_ch1(void){
  uint16_t i;
  float sumI,sqI;
  
  sumI = 0.0000;
  
  for(i=0;i<SIZEBUFFERCH;i++){
    ch1.bufferfilter[i] = (float)0.996 * (float)(ch1.hpf_lastfilter + (float)ch1.buffer[i] - (float)ch1.hpf_lastsample);
    
    ch1.hpf_lastfilter = ch1.bufferfilter[i];
    ch1.hpf_lastsample = ch1.buffer[i];
  
    
    //Root-mean-square method current
    //1) square current values
    sqI = (ch1.bufferfilter[i] * ch1.bufferfilter[i]);
    //2) sum
    sumI += sqI;   
  }
  ch1.rms = sqrt(sumI / (float)SIZEBUFFERCH);
}

void hpf_ch2(void){
  uint16_t i;
  float sumI,sqI;
  
  sumI = 0.0000;
  
  for(i=0;i<SIZEBUFFERCH;i++){
    ch2.bufferfilter[i] = (float)0.996 * (float)(ch2.hpf_lastfilter + (float)ch2.buffer[i] - (float)ch2.hpf_lastsample);
    
    ch2.hpf_lastfilter = ch2.bufferfilter[i];
    ch2.hpf_lastsample = ch2.buffer[i];
  
    
    //Root-mean-square method current
    //1) square current values
    sqI = (ch2.bufferfilter[i] * ch2.bufferfilter[i]);
    //2) sum
    sumI += sqI;   
  }
  ch2.rms = sqrt(sumI / (float)SIZEBUFFERCH);
}







//****CALLBACK****
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
 if(hadc->Instance == ADC1){
    __NOP();
    __NOP();
    __NOP();
    ADC1_flag = 1;
  }
}