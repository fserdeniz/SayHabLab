/* Includes ------------------------------------------------------------------*/
#include "telecommunication.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern ADC_HandleTypeDef hadc;
extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim2;

uint16_t adcValue_12bit;
uint16_t adcValue_10bit;
uint16_t adcValue_8bit;
uint16_t adcValue_6bit;
/* Private user code ---------------------------------------------------------*/

void mainCode(void)
{
	// Start TIM2 timer peripheral
	HAL_TIM_Base_Start(&htim2); // Timer is used to configure the sample rate
	
	// First part of the Lab Experiment
	// Use a potentiometer to set a voltage level input to ADC
	// get the ADC value when n = 12, 10, 8 and 6 bits
	
	setSampleRate(47, 9999); // Set the sample rate
	
	setADC_Resolution(ADC_RESOLUTION_12B); // Set ADC resolution
	HAL_ADC_Start(&hadc); // Start ADC peripheral
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY); // Wait until the ADC completes the conversion from analog to digital
	adcValue_12bit = HAL_ADC_GetValue(&hadc); // Get ADC output value (a digital value)
	HAL_ADC_Stop(&hadc); // Stop ADC peripheral in order to reconfigure it
	
	setADC_Resolution(ADC_RESOLUTION_10B); // Set ADC resolution
	HAL_ADC_Start(&hadc); // Start ADC peripheral
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY); // Wait until the ADC completes the conversion from analog to digital
	adcValue_10bit = HAL_ADC_GetValue(&hadc); // Get ADC output value (a digital value)
	HAL_ADC_Stop(&hadc); // Stop ADC peripheral in order to reconfigure it
	
	setADC_Resolution(ADC_RESOLUTION_8B); // Set ADC resolution
	HAL_ADC_Start(&hadc); // Start ADC peripheral
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY); // Wait until the ADC completes the conversion from analog to digital
	adcValue_8bit = HAL_ADC_GetValue(&hadc); // Get ADC output value (a digital value)
	HAL_ADC_Stop(&hadc); // Stop ADC peripheral in order to reconfigure it
	
	setADC_Resolution(ADC_RESOLUTION_6B); // Set ADC resolution
	HAL_ADC_Start(&hadc); // Start ADC peripheral
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY); // Wait until the ADC completes the conversion from analog to digital
	adcValue_6bit = HAL_ADC_GetValue(&hadc); // Get ADC output value (a digital value)
	HAL_ADC_Stop(&hadc); // Stop ADC peripheral in order to reconfigure it
	
	// Second part of the Lab Experiment
	// Use the ADC to sample an sinewave and the DAC to output the sampled digital value to an analog voltage again
	
	setADC_Resolution(ADC_RESOLUTION_12B); // Set ADC resolution
	setSampleRate(47, 99); // Set the sample rate
	
	// Start the required MCU perpherals: ADC and DAC	
	HAL_ADC_Start(&hadc); // ADC is used to convert an analog signal to a digital value
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1); // DAC is used to convert a digital value to an analog voltage
	
	// Sample the input signal continuously
	while(1)
	{
		HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY); // wait for ADC to complete the sampling
		adcValue_12bit = HAL_ADC_GetValue(&hadc); // get the sampled value
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, adcValue_12bit); // Set DAC input equal to adc output
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1); // convert the DAC input to an analog voltage
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
