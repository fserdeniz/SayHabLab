/* Includes ------------------------------------------------------------------*/
#include "telecommunication.h"

/* Private define ------------------------------------------------------------*/
#define N0		250 	// Sample count for one symbol
#define PI		3.14  // pi value to use in trigonometric functions

/* Private variables ---------------------------------------------------------*/
extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_dac_ch1;
extern DMA_HandleTypeDef hdma_dac_ch2;
extern TIM_HandleTypeDef htim2;
extern COMP_HandleTypeDef hcomp1;
extern COMP_HandleTypeDef hcomp2;

// Arrays to hold symbol samples
// First index of the array indicates the symbol order
// Second index of the array indicates the samples of each symbol
uint16_t	symbols[4][N0];

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
	
	// Start COMP1 comaparator peripheral, we will use it in demodulation
	HAL_COMP_Start(&hcomp1);
	
	// Start COMP2 comaparator peripheral, we will use it in demodulation
	HAL_COMP_Start(&hcomp2);
	
	// Start of the Lab Experiment
	// We send byte or bytes of arbitrary data
	// We use DAC_Channel_1 to output the BFSK modulation
	// We use DAC_Channel_2 to output the clock signal
	// We use COMP1 comparator to clear LPF output (demodulate BFSK)  
	// BFSK modulation pin PA4 (A2 on Nucleo-64)
	// Clock signal pin PA5 (D13 on Nucleo-64)
	// Frame Sync Signal pin PA9 (D8 on Nucleo-64)
	// COMP1 input(-) pin PA0 (A0 on Nucleo-64)
	// COMP1 input(+) pin PA1 (A1 on Nucleo-64)
	// COMP1 output pin PA6 (D12 on Nucleo-64)
	// COMP2 input(-) pin PA2 (D1 on Nucleo-64)
	// COMP2 input(+) pin PA3 (D0 on Nucleo-64)
	// COMP2 output pin PA7 (D11 on Nucleo-64)	
	
	// Our Sample Period, refer to setSampleRate() function to determine its equivalent sample time
	setSampleRate(1, 47); // Set the sample rate for DAC (500 kSample/s)
	
	// In the lines below we will construct tx symbols, each symbol contains 250 sample
	// Suppose that our data rate is (rb), our DAC sample rate is (fs) and sample count for each tx symbol is (N0)
	// So our data rate will be rb = fs / N0 = 500k / 250 = 2 kbit/s
	
	// Construct a symbol for "0" bit , step by step using n loop variable

	// Construct a symbol for "1" bit , step by step using n loop variable
		for (uint16_t n = 0; n < N0; n++) 
	{
		// Calculate the exact sample value of the symbol (floating number)
		
		// Send Carrier signal for "1" bit (BFSK Modulation)
		// Remember that our DAC can not output a negative voltage
		// So, we add a 0.5 offset coefficient and scaled the sinusoidal function with 0.49 coefficient
		// By this, out Carrier signal will oscillate approximately between 0V and 3.3V (offset = 0.5*3.3 = 1.65V and 2*0.49*3.3 = 3.23 Vpp)
		// Carrier frequency (f1), Total sample count for each Symbol (N0), Discrete frequency of the Carrier (n1) and the sample rate of the DAC (fs)
		// f1 = fs / (N0/n1), below we have n1 = 10 -> Carrier frequency f1 = 500kHz/(250/10) = 500kHz/25 = 20kHz
		// symbolValue = 0.49 * sinf(2*PI*n1*n/N0) + 0.5
		symbolValue = 0.49*sinf(2*PI*n/N0 + 3*PI/4) + 0.5;
		// Convert the exact sample value to 16-bit integer value
		// DAC perpheral of the MCU accepts only integer numbers
		symbols[0][n] = (uint16_t)(symbolValue * 4095);
			
	}
	for (uint16_t n = 0; n < N0; n++) 
	{
		// Calculate the exact sample value of the symbol (floating number)
		
		// Send Carrier signal for "1" bit (BFSK Modulation)
		// Remember that our DAC can not output a negative voltage
		// So, we add a 0.5 offset coefficient and scaled the sinusoidal function with 0.49 coefficient
		// By this, out Carrier signal will oscillate approximately between 0V and 3.3V (offset = 0.5*3.3 = 1.65V and 2*0.49*3.3 = 3.23 Vpp)
		// Carrier frequency (f1), Total sample count for each Symbol (N0), Discrete frequency of the Carrier (n1) and the sample rate of the DAC (fs)
		// f1 = fs / (N0/n1), below we have n1 = 10 -> Carrier frequency f1 = 500kHz/(250/10) = 500kHz/25 = 20kHz
		// symbolValue = 0.49 * sinf(2*PI*n1*n/N0) + 0.5
		symbolValue = 0.49*sinf(2*PI*n/N0 + 7*PI/4) + 0.5;
		// Convert the exact sample value to 16-bit integer value
		// DAC perpheral of the MCU accepts only integer numbers
		symbols[3][n] = (uint16_t)(symbolValue * 4095);
			
	}
	for (uint16_t n = 0; n < N0; n++) 
	{
		// Calculate the exact sample value of the symbol (floating number)
		
		// Send Carrier signal for "1" bit (BFSK Modulation)
		// Remember that our DAC can not output a negative voltage
		// So, we add a 0.5 offset coefficient and scaled the sinusoidal function with 0.49 coefficient
		// By this, out Carrier signal will oscillate approximately between 0V and 3.3V (offset = 0.5*3.3 = 1.65V and 2*0.49*3.3 = 3.23 Vpp)
		// Carrier frequency (f1), Total sample count for each Symbol (N0), Discrete frequency of the Carrier (n1) and the sample rate of the DAC (fs)
		// f1 = fs / (N0/n1), below we have n1 = 10 -> Carrier frequency f1 = 500kHz/(250/10) = 500kHz/25 = 20kHz
		// symbolValue = 0.49 * sinf(2*PI*n1*n/N0) + 0.5
		symbolValue = 0.49*sinf(2*PI*n/N0+ 5*PI/4) + 0.5;
		// Convert the exact sample value to 16-bit integer value
		// DAC perpheral of the MCU accepts only integer numbers
		symbols[1][n] = (uint16_t)(symbolValue * 4095);
			
	}
		for (uint16_t n = 0; n < N0; n++) 
	{
		// Calculate the exact sample value of the symbol (floating number)
		
		// Send Carrier signal for "1" bit (BFSK Modulation)
		// Remember that our DAC can not output a negative voltage
		// So, we add a 0.5 offset coefficient and scaled the sinusoidal function with 0.49 coefficient
		// By this, out Carrier signal will oscillate approximately between 0V and 3.3V (offset = 0.5*3.3 = 1.65V and 2*0.49*3.3 = 3.23 Vpp)
		// Carrier frequency (f1), Total sample count for each Symbol (N0), Discrete frequency of the Carrier (n1) and the sample rate of the DAC (fs)
		// f1 = fs / (N0/n1), below we have n1 = 10 -> Carrier frequency f1 = 500kHz/(250/10) = 500kHz/25 = 20kHz
		// symbolValue = 0.49 * sinf(2*PI*n1*n/N0) + 0.5
		symbolValue = 0.49*sinf(2*PI*n/N0 + PI/4) + 0.5;
		// Convert the exact sample value to 16-bit integer value
		// DAC perpheral of the MCU accepts only integer numbers
		symbols[2][n] = (uint16_t)(symbolValue * 4095);
	}


	
	// Construct clock signal, step by step using n loop variable

	
	// Determine transmit data value
	txData = 0x8C;
	
	// The while loop below to send data continuously
	while(1)
	{
		// Frame Sync Signal, SET frame sync GPIO pin (PA9), wait for some time then RESET it
		htim2.Instance->CNT = 0; // Set timer counter register to zero (to adjust waiting time precisely)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Sync start
		while(htim2.Instance->CNT < (htim2.Instance->ARR - 4)); // wait for one DAC sampling period
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);	// Sync end
		
		// Send txData bit by bit
		for (i = 0; i < 8; i=i+2) 
		{
			// Determine the symbol to send according to current txData bit (LSB First)
			txSymbol = ((txData >> i) & 0x03);
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