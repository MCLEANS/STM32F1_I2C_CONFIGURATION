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
#include "Timerconfiguration.h"

#define PRESCALER 66
#define ARR_VALUE 1098

#define BUFFER_SIZE 4

custom_libraries::Timer_configuration timer(TIM1,PRESCALER,ARR_VALUE);

uint16_t tick_ms = 0;

uint8_t buffer[BUFFER_SIZE];

int test = 0;

int seconds = 0;
int minutes = 0;
int hours = 0;

void delay_ms(uint16_t ms){
	tick_ms = 0;
	while(tick_ms < ms){}
}

uint8_t BCD_to_decimal(uint8_t val){
	return((val /16 * 10) + (val %16));
}

uint8_t decimal_to_BCD(uint8_t val){
	return((val /10 * 16) + (val %10));
}

void read(uint8_t address,uint8_t len){
	uint32_t temp = 0;
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	//ENABLE I2C DMA
	I2C1->CR2 |= I2C_CR2_DMAEN;
	//ENABLE ACKS
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR2 |= I2C_CR2_LAST;
	//CONFIGURE DMA
	DMA1_Channel7 ->CMAR = (uint32_t) buffer;
	DMA1_Channel7 -> CPAR = (uint32_t) &I2C1->DR;
	DMA1_Channel7->CNDTR = len;
	DMA1_Channel7 ->CCR |= DMA_CCR7_TCIE | DMA_CCR7_MINC | DMA_CCR7_EN |DMA_CCR7_CIRC;
	//SEND START BIT
	I2C1->CR1 |= I2C_CR1_START;
	//WAIT FOR START BIT TO BE SENT
	while(!(I2C1->SR1 & I2C_SR1_SB)){}
	//SEND DEVICE ADDRESS
	I2C1->DR = address+1;
	//WAIT FOR ADDRESS TO BE SENT
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}
	temp = I2C1->SR2;
	//WAIT UNTIL DMA TRANFER IS COMPLETE
	while((DMA1->ISR & DMA_ISR_TCIF7) == 0){}
	//CLEAR TRANSFER COMPLETE FLAG
	DMA1->IFCR |= DMA_IFCR_CTCIF7;
	//SEND STOP BIT
	I2C1->CR1 |= I2C_CR1_STOP;



}

void write_single(uint8_t address,uint8_t mem,uint8_t data){
	uint32_t temp;
	//GENERATE START CONDITION
	I2C1->CR1 |= I2C_CR1_START;
	//WAIT UNTIL START BIT IS SENT
	while(!(I2C1->SR1 & I2C_SR1_SB)){};
	//SEND ADDRESS
	I2C1->DR = address;
	//WAIT UNTIL ADDRESS IS SENT
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}
	//READ SR1 AND SR2 TO CLEAR THE BIT
	temp = I2C1->SR2;
	//ADDRESS TO WRITE TO
	I2C1->DR = mem;
	//WAIT FOR TRANSFER TO COMPLETE
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}
	//DATA TO WRITE IN ADDRESS
	I2C1->DR = data;
	//WAIT FOR TRANSFER TO COMPLETE
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}
	//SET STOP BIT
	I2C1->CR1 |= I2C_CR1_STOP;

}

void set_pointer(uint8_t address,uint8_t mem){
	uint32_t temp;
	//GENERATE START CONDITION
	I2C1->CR1 |= I2C_CR1_START;
	//WAIT UNTIL START BIT IS SENT
	while(!(I2C1->SR1 & I2C_SR1_SB)){};
	//SEND ADDRESS
	I2C1->DR = address;
	//WAIT UNTIL ADDRESS IS SENT
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}
	//READ SR1 AND SR2 TO CLEAR THE BIT
	temp = I2C1->SR2;
	//ADDRESS TO WRITE TO
	I2C1->DR = mem;
	//WAIT FOR TRANSFER TO COMPLETE
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}
	//SET STOP BIT
	I2C1->CR1 |= I2C_CR1_STOP;

}



extern "C" void TIM1_UP_IRQHandler(void){
	if(TIM1->SR & TIM_SR_UIF){
		TIM1->SR &= ~TIM_SR_UIF;
		tick_ms++;
	}
}


int main(void)
{

	//ENABLE I2C RCC

	RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
	I2C1->CR2 = 0x00;
	I2C1->CR1 = 0x00;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	//ENABLE GPIO RCC
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	//ENABLE DMA RCC


	//SET PERIPHERAL CLOCK REQUENCY FOR I2C
	I2C1->CR2 |= 36;

	//SET CLOCK CONTROL REGISTER IN Fm/Sm
	I2C1->CCR |= 180;

	//SET I2C RISE TIME
	I2C1->TRISE = 37;



	//SET I2C PINS TO ALTERNATE OPEN DRAIN AND ENABLE PULL_UP
	GPIOB->CRL |= GPIO_CRL_MODE6;
	GPIOB->CRL  |= GPIO_CRL_CNF6;
	GPIOB->CRL |= GPIO_CRL_MODE7;
	GPIOB->CRL  |= GPIO_CRL_CNF7;
	GPIOB->ODR |= GPIO_ODR_ODR6;
	GPIOB->ODR |= GPIO_ODR_ODR7;

	//ENABLE THE PERIPHERAL, MUST BE DONE LAST
	I2C1->CR1 |= I2C_CR1_PE;

	//ENABLE TIMER NVIC
	NVIC_SetPriority(TIM1_UP_IRQn,0X00);
	NVIC_EnableIRQ(TIM1_UP_IRQn);

	//write_single(0xD0,0x00,0x00);
	//write_single(0xD0,0x01,0x00);
	//write_single(0xD0,0x02,0x00);
	////write_single(0xD0,0x03,0x07);
	//write_single(0xD0,0x00,0x07);
	//(0xD0,0X00);
	//delay_ms(1);
	//ENABLE ACKS
	//set_pointer(0xD0,0X00);
	//read(0xD0,4);

	while(1){

		set_pointer(0xD0,0X00);
		read(0xD0,4);
		seconds = BCD_to_decimal(buffer[0]);
		minutes = BCD_to_decimal(buffer[1]);
		hours = BCD_to_decimal(buffer[2]);
		test+=1;


	}
}
