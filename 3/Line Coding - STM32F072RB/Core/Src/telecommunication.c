/* Includes ------------------------------------------------------------------*/
#include "telecommunication.h"

/* Private define ------------------------------------------------------------*/
#define N0		100 // Sample count for one symbol

/* Private variables ---------------------------------------------------------*/
extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_dac_ch1;
extern DMA_HandleTypeDef hdma_dac_ch2;
extern TIM_HandleTypeDef htim2;

// Arrays to hold symbol samples
// First index of the array indicates the symbol order
// Second index of the array indicates the samples of each symbol
uint16_t	symbols[2][N0];

// Generate Clock signal symbol
uint16_t	clock[N0];

// Current sample value of the symbol (it is a float number and must be converted to integer)
float	symbolValue; 

// Data to transmit
uint8_t txData;

// Symbol to transmit
uint8_t txSymbol;

// Loop Counter
uint8_t i = 0;

/* Private user code ---------------------------------------------------------*/

void mainCode(void)
{
	// Start TIM2 timer peripheral
	HAL_TIM_Base_Start(&htim2); // Timer is used to configure the sample rate
	
	// Start of the Lab Experiment
	// We send byte or bytes of arbitrary data
	// We use DAC_Channel_1 to output the desired line coding
	// We use DAC_Channel_2 to output the clock signal
	// Line Coding pin PA4 (A2 on Nucleo-64)
	// Clock signal pin PA5 (D13 on Nucleo-64)
	// Frame Sync Signal pin PA9 (D8 on Nucleo-64)
	
	// Our Sample Period, refer to setSampleRate() function to determine its equivalent sample time
	setSampleRate(1, 47); // Set the sample rate for DAC (500 kSample/s)
	
	// In the lines below we will construct tx symbols, each symbol contains 100 sample
	// Suppose that our data rate is (rb), our DAC sample rate is (fs) and sample count for each tx symbol is (N0)
	// So our data rate will be rb = fs / N0 = 500k / 100 = 5 kbit/s
	
	// Construct a symbol for "0" bit , step by step using n loop variable
	for (uint16_t n = 0; n < N0; n++) 
	{
		// Calculate the exact sample value of the symbol (floating number)
		// Logic low signal (voltage level = 0V) for "0" bit (it is valid for both NRZ and RZ)
		symbolValue = 0.0;
		// Convert the exact sample value to 16-bit integer value
		// DAC perpheral of the MCU accepts only integer numbers
		symbols[0][n] = (uint16_t)(symbolValue * 4095);
	}
	
	// Construct a symbol for "1" bit , step by step using n loop variable
	for (uint16_t n = 0; n < N0; n++) 
	{
		// Calculate the exact sample value of the symbol (floating number)
		
		// The line below constructs "1" bit for NRZ line coding
//		symbolValue = 1.0;
		
		// The lines below construct "1" bit for RZ line coding (duty cycle = 50%)
		// we have sample count for each symbol N0 = 100 -> 50% of N0 = 50
		if(n < (50))
		{
			symbolValue = 1.0;
		}
		else
		{
			symbolValue = 0.0;
		}		
		
		// Convert the exact sample value to 16-bit integer value
		// DAC perpheral of the MCU accepts only integer numbers
		symbols[1][n] = (uint16_t)(symbolValue * 4095);
	}
	
	// Construct clock signal, step by step using n loop variable
	for (uint16_t n = 0; n < N0; n++) 
	{
		// Calculate the exact sample value of the symbol (floating number)
		// Clock signal is at high logic level half the period and at logic low level the remaining of the period
		if(n < (N0/2))
		{
			symbolValue = 1.0;
		}
		else
		{
			symbolValue = 0.0;
		}
		// Convert the exact sample value to 16-bit integer value
		// DAC perpheral of the MCU accepts only integer numbers
		clock[n] = (uint16_t)(symbolValue * 4095);
	}	

	
	// Determine transmit data value
	txData = 0xA5;
	
	// The while loop below to send data continuously
	while(1)
	{
		// Frame Sync Signal, SET frame sync GPIO pin (PA9), wait for some time then RESET it
		htim2.Instance->CNT = 0; // Set timer counter register to zero (to adjust waiting time precisely)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Sync start
		while(htim2.Instance->CNT < (htim2.Instance->ARR - 2)); // wait for one DAC sampling period
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);	// Sync end
		
		// Send txData bit by bit
		for (i = 0; i < 8; i++) 
		{
			// Determine the symbol to send according to current txData bit (LSB First)
			txSymbol = ((txData >> i) & 0x01);
			// Pay attention that we must transmit the symbol and clock simultaneously
			// Start digital to analog coversion on DAC Channel 1 (for symbol transmission)
			HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) &symbols[txSymbol][0], N0, DAC_ALIGN_12B_R);
			// Start digital to analog coversion on DAC Channel 2 (for clock signal)
			HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*) &clock[0], N0, DAC_ALIGN_12B_R);		
			// wait until MCU DAC peripheral converts all digital data to analog signal
			while(HAL_DAC_GetState(&hdac) == HAL_DAC_STATE_BUSY);
		}			
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
