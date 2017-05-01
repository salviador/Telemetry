#include "serialchart.h"
#include <string.h>

UART_HandleTypeDef huart2;



void init_serialChart(void){
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART2_CLK_ENABLE();

  /**USART2 GPIO Configuration    
  PA2     ------> USART2_TX
  PA15     ------> USART2_RX 
  */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 460800;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart2);
}

void serialChart_write(uint8_t *data){
  HAL_UART_Transmit(&huart2, data, strlen((const char *)data), HAL_MAX_DELAY);
}

uint8_t serialChart_wait_receiveByte(uint8_t *data){
  HAL_UART_Receive(&huart2, data, 1, HAL_MAX_DELAY);
  return 1;
}


void serialChart_sendJsonTelemetry(struct CH channel1, struct CH channel2, uint16_t ch1size){
  uint8_t buffer[100];
  uint16_t i;
  
  //{
  sprintf((char *)buffer, "{");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);

  //"ch1" : [30,20,20,20,..... ]
  sprintf((char *)buffer, "\"ch1\":[");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
  for(i=0;i<ch1size;i++){
    sprintf((char *)buffer, "%d", channel1.buffer[i]);
    HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
    if(i != ch1size-1){
      sprintf((char *)buffer, ",");
      HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
    }
  }
  sprintf((char *)buffer, "]");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
  
  //"ch1filter" : [30,20,20,20,..... ]
  sprintf((char *)buffer, ",\"ch1filter\":[");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
  for(i=0;i<ch1size;i++){
    sprintf((char *)buffer, "%.1f", channel1.bufferfilter[i]);
    HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
    if(i != ch1size-1){
      sprintf((char *)buffer, ",");
      HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
    }
  }
  sprintf((char *)buffer, "]");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY); 

  //"rms" : 10.2
  sprintf((char *)buffer, ",\"rms\":%.1f", channel1.rms);
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);


  
  
  
  
  
  
  
  //"ch2" : [30,20,20,20,..... ]
  sprintf((char *)buffer, ",\"ch2\":[");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
  for(i=0;i<ch1size;i++){
    sprintf((char *)buffer, "%d", channel2.buffer[i]);
    HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
    if(i != ch1size-1){
      sprintf((char *)buffer, ",");
      HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
    }
  }
  sprintf((char *)buffer, "]");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
  
  //"ch2filter" : [30,20,20,20,..... ]
  sprintf((char *)buffer, ",\"ch2filter\":[");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
  for(i=0;i<ch1size;i++){
    sprintf((char *)buffer, "%.1f", channel2.bufferfilter[i]);
    HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
    if(i != ch1size-1){
      sprintf((char *)buffer, ",");
      HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
    }
  }
  sprintf((char *)buffer, "]");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY); 

  //"rms" : 10.2
  sprintf((char *)buffer, ",\"rms2\":%.1f", channel2.rms);
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);
  






  
  

  //}
  sprintf((char *)buffer, "}\r\n");
  HAL_UART_Transmit(&huart2, buffer, strlen((const char *)buffer), HAL_MAX_DELAY);

}












