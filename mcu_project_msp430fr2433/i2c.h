/*
 * i2c.h
 *
 *  Created on: Jul 20, 2021
 *      Author: Dexter
 */

#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

enum eI2C_MODE
{
    I2C_IDLE_MODE,
    I2C_NACK_MODE,
    I2C_TX_REG_ADDRESS_MODE,
    I2C_RX_REG_ADDRESS_MODE,
    I2C_TX_DATA_MODE,
    I2C_RX_DATA_MODE,
    I2C_SWITCH_TO_RX_MODE,
    I2C_SWITCH_TO_TX_MODE,
    I2C_TIMEOUT_MODE
};

enum eI2C_MODE i2c_write(uint8_t dev_addr, uint8_t *data, uint16_t count);
enum eI2C_MODE i2c_write_reg1(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t count);
enum eI2C_MODE i2c_write_reg2(uint8_t dev_addr, uint16_t reg, uint8_t *data, uint16_t count);
enum eI2C_MODE i2c_write_nostop(uint8_t dev_addr, uint8_t *data, uint16_t count);
enum eI2C_MODE i2c_write_cont(uint8_t dev_addr, uint8_t *data, uint16_t count);

enum eI2C_MODE i2c_read(uint8_t dev_addr, uint8_t* dest, uint16_t count);
enum eI2C_MODE i2c_read_reg1(uint8_t dev_addr, uint8_t reg, uint8_t* dest, uint16_t count);
enum eI2C_MODE i2c_read_reg2(uint8_t dev_addr, uint16_t reg, uint8_t* dest, uint16_t count);

bool i2c_got_nack(void);
void i2c_init(void);
void i2c_isr(void);


#endif /* I2C_H_ */
