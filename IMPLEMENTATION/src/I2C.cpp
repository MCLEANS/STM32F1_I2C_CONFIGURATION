/*
 * I2C.cpp
 *
 *  Created on: Aug 3, 2020
 *      Author: MCLEANS
 */

#include "I2C.h"

namespace custom_libraries {

I2C_::I2C_(I2C_TypeDef *_I2C,
			GPIO_TypeDef *GPIO,
			uint8_t SDA,
			uint8_t SCL,
			Mode mode): _I2C(_I2C),
						GPIO(GPIO),
						SDA(SDA),
						SCL(SCL),
						mode(mode){
	//INITIALIZE I2C

	//INITIALIZE I2C RCC ENABLE BITS TO OFF
	if(_I2C == I2C1) RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
	if(_I2C == I2C2) RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN;

	//INITIALIZE I2C CONTROL REGISTERS TO 0X00
	_I2C->CR2 = 0x00;
	_I2C->CR1 = 0x00;

	//ENABLE I2C RCC
	if(_I2C == I2C1) RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	if(_I2C == I2C2) RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

	//ENABLE GPIO RCC
	if(GPIO == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	if(GPIO == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	if(GPIO == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	if(GPIO == GPIOD) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	if(GPIO == GPIOE) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;

	//SET I2C PINS TO ALTERNATE OPEN DRAIN AND ENABLE PULL_UP
	if(SCL < 8){
			GPIO->CRL &= ~(1 << (((SCL*4)+2)));
			GPIO->CRL &= ~(1 << (((SCL*4)+2)+1));
		}
		else{
			GPIO->CRH &= ~(1 << ((((SCL-8)*4)+2)));
			GPIO->CRH |= (1 << ((((SCL-8)*4)+2)+1));
		}

	if(SDA < 8){
			GPIO->CRL &= ~(1 << (((SDA*4)+2)));
			GPIO->CRL |= (1 << (((SDA*4)+2)+1));
		}
		else{
			GPIO->CRH &= ~(1 << ((((SDA-8)*4)+2)));
			GPIO->CRH |= (1 << ((((SDA-8)*4)+2)+1));
		}

	GPIO->ODR |= (1 << SCL);
	GPIO->ODR |= (1 << SDA);

	//SET PERIPHERAL CLOCK REQUENCY FOR I2C (I2C IS ON APB1 AT 36MHz)
	//CHANGE THIS TO THE CLOCK FREQUENCY OF YOUR APB1 BUS
	I2C1->CR2 |= APB1_FREQ;

	//SET CLOCK CONTROL REGISTER IN Fm/Sm
	int I2C_FREQUENCY = 0;
	if(mode == standard) I2C_FREQUENCY = 100;
	if(mode == fast) I2C_FREQUENCY = 400;

	float I2C_PERIOD = (1/I2C_FREQUENCY);
	double APB_BUS_PERIOD = (1/APB1_FREQ);

	int CCR_VALUE = ((I2C_PERIOD/2)/APB_BUS_PERIOD);
	I2C1->CCR |= CCR_VALUE;

	//SET I2C RISE TIME
	signed long  RISE_TIME =(((1/1000000)/APB_BUS_PERIOD)+1);
	I2C1->TRISE = RISE_TIME;

	//ENABLE THE PERIPHERAL, MUST BE DONE LAST
	I2C1->CR1 |= I2C_CR1_PE;


}

I2C_::~I2C_() {
	// TODO Auto-generated destructor stub
}

} /* namespace custom_libraries */
