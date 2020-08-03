/*
 * I2C.h
 *
 *  Created on: Aug 3, 2020
 *      Author: MCLEANS
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f10x.h"



namespace custom_libraries {

#define APB1_FREQ 36
#define APB2_FREQ 72

enum Mode{
	standard, //100KHz
	fast //400KHz
};

class I2C_ {
private:
	I2C_TypeDef *_I2C;
	GPIO_TypeDef *GPIO;
	uint8_t SDA;
	uint8_t SCL;
	Mode mode;

private:
public:
public:
	I2C_(I2C_TypeDef *_I2C,
		GPIO_TypeDef *GPIO,
		uint8_t SDA,
		uint8_t SCL,
		Mode mode);
	void read_bytes(uint8_t address,uint8_t *buffer,uint8_t len);
	void write_byte(uint8_t address,uint8_t mem,uint8_t data);
	void write_bytes(uint8_t address,uint8_t mem,uint8_t *data,uint16_t len);
	void write_memp(uint8_t address,uint8_t mem);

	~I2C_();
};

} /* namespace custom_libraries */

#endif /* I2C_H_ */
