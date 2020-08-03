/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "I2C.h"

custom_libraries::I2C_ i2c(I2C1,GPIOB,7,6,custom_libraries::standard);

#define BUFFER_SIZE 4
uint8_t buffer[BUFFER_SIZE];

int seconds = 0;
int minutes = 0;
int hours = 0;

int main(void)
{

	uint8_t data[4] = {0x00,0x01,0x02,0x03};
	i2c.write_bytes(0xD0,0x00,data,4);

	while(1){

		i2c.write_memp(0xD0,0X00);
		i2c.read_bytes(0xD0,buffer,BUFFER_SIZE);
		seconds = i2c.BCD_to_decimal(buffer[0]);
		minutes = i2c.BCD_to_decimal(buffer[1]);
		hours = i2c.BCD_to_decimal(buffer[2]);

	}
}
