User
#include "main.h"
#include <string.h>
#include <stdio.h>

#define VOLTAGE_REFERENCE 3.3f

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);

uint32_t adc_value = 0;
float voltage = 0.0f;

void sendVoltage(uint32_t value) {
    char buffer[20];
    sprintf(buffer, "%.2f\r\n", value * VOLTAGE_REFERENCE / 4095.0f);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
 

  SystemClock_Config();


  MX_USART2_UART_Init();
  MX_ADC1_Init();

  HAL_ADC_Start(&hadc1);
  
  while (1)
  {
   
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    adc_value = HAL_ADC_GetValue(&hadc1);
    sendVoltage(adc_value);
    HAL_Delay(600);
  }
  
}
