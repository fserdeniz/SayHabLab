/* Includes ------------------------------------------------------------------*/
#include "telecommunication.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern ADC_HandleTypeDef hadc;
extern TIM_HandleTypeDef htim2;

uint32_t samplePeriod;
uint16_t adcValue;
uint8_t bitToSend;
/* Private user code ---------------------------------------------------------*/

void mainCode(void)
{
	// Start TIM2 timer peripheral
	HAL_TIM_Base_Start(&htim2); // Timer is used to configure the sample rate
	
	// Start of the Lab Experiment
	// Use the ADC to sample an analog voltage
	// Then we send PCM bits of the sampled signal (adcValue_8bit)
	// ADC input pin PC0 (A5 on Nucleo-64)
	// PCM output pin PA8 (D7 on Nucleo-64)
	// Clock signal pin PB10 (D6 on Nucleo-64)
	// Frame Sync Signal pin PA9 (D8 on Nucleo-64)
	
	setADC_Resolution(ADC_RESOLUTION_8B); // Set ADC resolution
	// Our Sample Period, refer to setSampleRate() function to determine its equivalent sample time
	samplePeriod = 999; // sample time = 1 ms
	setSampleRate(47, samplePeriod); // Set the sample rate
	
	// Start the required MCU perpherals: ADC	
	HAL_ADC_Start(&hadc); // ADC is used to convert an analog signal to a digital value
	
	// Sample the input signal continuously
	while(1)
	{
		// Frame Sync Signal, SET frame sync GPIO pin (PA9), wait for some time then RESET it
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Sync start
		
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // keep PCM output pin at logic zero
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // keep Clock signal pin at logic zero
		
		while(htim2.Instance->CNT % samplePeriod < (samplePeriod/10)); // wait for one tenth of sampling period
		
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // Sync end
		
		HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY); // wait for ADC to complete the sampling
		adcValue = HAL_ADC_GetValue(&hadc); // get the sampled value
		
		for(uint8_t i = 0; i < 8; i++) // i variable stops at 8 beacause of 8-bits
		{		
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET); // Set Clock signal to logic one
			
			// Send Most Significant Bit (MSB) fisrt
			bitToSend = (adcValue << i) & 0x80; // Mask with 0x80 to determine the MSB
			if(bitToSend) bitToSend = 1; // if bit value is not equal to zero, bit value is one
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, bitToSend); // Set PCM output pin according to current bit value			
			
			// to send 8-bits in one sampling periode we must set bit = period / 8
			while(htim2.Instance->CNT % (samplePeriod/16) < ((samplePeriod/16) - 1)); // wait for period/16
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // // Set Clock signal to logic zero
			while(htim2.Instance->CNT % (samplePeriod/16) < ((samplePeriod/16) - 1)); // wait for period/16
			
		}		
	}
	
}

// This function set the ADC resolution in n bits
// Remember that ADC quantization levels equal to 2^n
// In STM32F091RC the ADC resolution n can be: 12, 10, 8 or 6 bits
// this can be configured using the predefined constants:
// ADC_RESOLUTION_12B, ADC_RESOLUTION_10B, ADC_RESOLUTION_8B or ADC_RESOLUTION_6B
void setADC_Resolution(uint32_t n)
{
  hadc.Init.Resolution = n;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {

  }
}

// This function sets the sample rate of the ADC/DAC
// Keep in mind that sample rate equals = (48MHz)/((prescaler + 1)*(period + 1))
void setSampleRate(uint16_t prescaler, uint32_t period)
{
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = prescaler;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = period;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
		
  }	
}
