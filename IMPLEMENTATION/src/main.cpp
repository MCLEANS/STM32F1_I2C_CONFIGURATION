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
			

int main(void)
{

	//ENABLE I2C RCC
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	//ENABLE GPIO RCC
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	//SET PERIPHERAL CLOCK REQUENCY FOR I2C
	I2C1->CR2 |= 36;

	//SET CLOCK CONTROL REGISTER IN Fm/Sm
	I2C1->CCR |= 180;

	//SET I2C RISE TIME
	I2C1->TRISE = 37;

	//ENABLE ACKS
	I2C1->CR1 |= I2C_CR1_ACK;

	//SET I2C PINS TO ALTERNATE OPEN DRAIN AND ENABLE PULL_UP
	GPIOB->CRL |= GPIO_CRL_MODE6;
	GPIOB->CRL  |= GPIO_CRL_CNF6;

	GPIOB->CRL |= GPIO_CRL_MODE7;
	GPIOB->CRL  |= GPIO_CRL_CNF7;

	//ENABLE THE PERIPHERAL, MUST BE DONE LAST
	I2C1->CR1 |= I2C_CR1_PE;






	while(1){

	}
}
