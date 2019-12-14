/*
 * display.c
 *
 *  Created on: Dec 8, 2019
 *      Author: cabantous
 */

#include "../Inc/display.h"

void SPI_write(uint8_t add, uint8_t data)
{
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 1);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 0);

	HAL_SPI_Transmit(&hspi1,&add, sizeof(uint8_t), 10);
	HAL_SPI_Transmit(&hspi1,&data, sizeof(uint8_t), 10);

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 1);
}

void MAX7219_Config(void)
{
	/* Max 7219 configuration */

	SPI_write(0x0C, 0x01);//Shutdown à 1 (Mode activé)
	SPI_write(0x0A, 0x0F);//Intensité lumineuse au MAAAX

	SPI_write(0x09, 0x0F);
	SPI_write(0x0B, 0x03);//Utiliser 4 afficheurs (Scan limit à 0x03 : Afficher digits 0 1 2 3)
	SPI_write(0x0F, 0x00);//Display Test à 0 : Mode test off, mode affichage normal on
	SPI_write(0x01, 0x0F);
	SPI_write(0x02, 0x0F);
	SPI_write(0x03, 0x0F);
	SPI_write(0x04, 0x0F);

}

void Segment_Display(uint16_t fq)
{
	uint8_t unite= 0, dizaine = 0, centaine = 0, millier = 0;

	unite = (fq%10);
	dizaine = ((fq%100)-(fq%10))/10;
	centaine = ((fq%1000)-(fq%100))/100;
	millier = ((fq%10000)-(fq%1000))/1000;


	//	printf("fq = %d%d%d%d\n",millier,centaine,dizaine,unite);

	SPI_write(0x04,unite);
	SPI_write(0x03,dizaine);
	SPI_write(0x02, centaine);
	SPI_write(0x01, millier);
}
