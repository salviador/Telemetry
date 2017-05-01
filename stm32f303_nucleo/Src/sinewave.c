#include "sinewave.h"

DMA_HandleTypeDef hdma_dac1_ch1;
DAC_HandleTypeDef hdac1;
TIM_HandleTypeDef htim2;

//http://00xnor.blogspot.it/2014/01/6-stm32-f4-dac-dma-waveform-generator.html

#define   OUT_FREQ          10000                                // Output waveform frequency
#define   SINE_RES          128                                  // Waveform resolution
#define   CNT_FREQ          64000000                             // TIM6 counter clock (prescaled APB1)
#define   TIM_PERIOD        ((CNT_FREQ)/((SINE_RES)*(OUT_FREQ))) // Autoreload reg value

/*
const uint16_t function[SINE_RES] = { 2048, 2145, 2242, 2339, 2435, 2530, 2624, 2717, 2808, 2897, 
                                      2984, 3069, 3151, 3230, 3307, 3381, 3451, 3518, 3581, 3640, 
                                      3696, 3748, 3795, 3838, 3877, 3911, 3941, 3966, 3986, 4002, 
                                      4013, 4019, 4020, 4016, 4008, 3995, 3977, 3954, 3926, 3894, 
                                      3858, 3817, 3772, 3722, 3669, 3611, 3550, 3485, 3416, 3344, 
                                      3269, 3191, 3110, 3027, 2941, 2853, 2763, 2671, 2578, 2483, 
                                      2387, 2291, 2194, 2096, 1999, 1901, 1804, 1708, 1612, 1517, 
                                      1424, 1332, 1242, 1154, 1068, 985, 904, 826, 751, 679, 
                                      610, 545, 484, 426, 373, 323, 278, 237, 201, 169, 
                                      141, 118, 100, 87, 79, 75, 76, 82, 93, 109, 
                                      129, 154, 184, 218, 257, 300, 347, 399, 455, 514, 
                                      577, 644, 714, 788, 865, 944, 1026, 1111, 1198, 1287, 
                                      1378, 1471, 1565, 1660, 1756, 1853, 1950, 2047 };  

*/


const uint16_t function[SINE_RES] = { 2048, 2141, 2235, 2328, 2421, 2512, 2603, 2692, 2779, 2865, 
                                      2948, 3030, 3109, 3185, 3259, 3330, 3398, 3462, 3523, 3580, 
                                      3633, 3683, 3728, 3770, 3807, 3840, 3869, 3893, 3912, 3927, 
                                      3938, 3944, 3945, 3941, 3933, 3921, 3903, 3881, 3855, 3824, 
                                      3789, 3750, 3706, 3659, 3607, 3552, 3493, 3430, 3364, 3295, 
                                      3223, 3148, 3070, 2989, 2907, 2822, 2736, 2647, 2557, 2466, 
                                      2374, 2282, 2188, 2094, 2001, 1907, 1813, 1721, 1629, 1538, 
                                      1448, 1359, 1273, 1188, 1106, 1025, 947, 872, 800, 731, 
                                      665, 602, 543, 488, 436, 389, 345, 306, 271, 240, 
                                      214, 192, 174, 162, 154, 150, 151, 157, 168, 183, 
                                      202, 226, 255, 288, 325, 367, 412, 462, 515, 572, 
                                      633, 697, 765, 836, 910, 986, 1065, 1147, 1230, 1316, 
                                      1403, 1492, 1583, 1674, 1767, 1860, 1954, 2047 };










void init_gen_sinewave(void){
  GPIO_InitTypeDef GPIO_InitStruct;
  DAC_ChannelConfTypeDef sConfig;
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_DAC1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();

    /**DAC1 GPIO Configuration    
    PA4     ------> DAC1_OUT1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* TIM2 init function */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = (uint16_t)TIM_PERIOD; 
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; //TIM_AUTORELOAD_PRELOAD_ENABLE; //TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
    
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  //  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  //  HAL_NVIC_EnableIRQ(TIM2_IRQn);  
    
    //Dac Inizialize
    hdac1.Instance = DAC1;
    HAL_DAC_Init(&hdac1);
    sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1);
  
    /* Peripheral DMA init*/
    hdma_dac1_ch1.Instance = DMA1_Channel3;
    hdma_dac1_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_dac1_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dac1_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dac1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dac1_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dac1_ch1.Init.Mode = DMA_CIRCULAR;
    hdma_dac1_ch1.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_dac1_ch1);
    __HAL_DMA_REMAP_CHANNEL_ENABLE(HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3);
    __HAL_LINKDMA(&hdac1,DMA_Handle1,hdma_dac1_ch1);
    
    /* DMA1_Channel3_IRQn interrupt configuration */
 //   HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
 //   HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

    
//    HAL_TIM_Base_Start_IT(&htim2);
      HAL_TIM_Base_Start(&htim2);

    //HAL_DMA_Start_IT(&hdma_dac1_ch1, (uint32_t)function,(uint32_t)&hdac1.Instance->DHR12R1, SINE_RES);
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)function, SINE_RES, DAC_ALIGN_12B_R);
 
}