/*
 * display.h
 *
 *  Created on: Dec 8, 2019
 *      Author: cabantous
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "spi.h"
#include <stdio.h>
void MAX7219_Config(void);
void Segment_Display(uint16_t fq);
void SPI_write(uint8_t add, uint8_t data);

#endif /* DISPLAY_H_ */


//#ifndef __MAX7219_H__
//#define __MAX7219_H__
//
////#include "stm32f10x.h"
//#include "stm32l1xx_hal.h"
//#include "gpio.h"
//
//void init_max7219(uint8_t intensivity);
//
//void displayTemperature(uint8_t* array);
//void displayTime(uint8_t* array);
//void displayEncoder(uint8_t* array);
//
//void max7219_setIntensivity(uint8_t intensivity);
//
//#endif /* __MAX7219_H__ */
