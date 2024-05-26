/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TELECOMMUNICATIONS_H
#define __TELECOMMUNICATIONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "math.h"

/* Private defines -----------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
void mainCode(void);
void setSampleRate(uint16_t prescaler, uint32_t period);
void setADC_Resolution(uint32_t n);

#endif /* __TELECOMMUNICATIONS_H */
