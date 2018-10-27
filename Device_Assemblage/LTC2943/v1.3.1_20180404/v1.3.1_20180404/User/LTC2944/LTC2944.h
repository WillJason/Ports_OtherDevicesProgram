#ifndef   _LTC2944_H
#define	 _LTC2944_H

#include "stm32f10x.h"

//LTC2944 I2C Address
#define LTC2944_I2C_ADDRESS         0x64  
#define LTC2944_I2C_ALERT_RESPONSE  0x0C

//! @name I2C READ and WRITE BITS
//! @{
//! Eighth bit (LSB) of I2C address indicates a "read" or "write".
//! (The first seven bits are the 7-bit I2C address.)
#define I2C_READ_BIT    0x01
#define I2C_WRITE_BIT   0x00
//! @}

//! @name ACK OR NACK PARAMETER PASSED TO I2C_READ
//! @{
#define WITH_ACK  0  //!<  Use with i2c_read(WITH_ACK) to read with an acknowledge
#define WITH_NACK 1  //!<  Use with i2c_read(WITH_NACK) to read without an acknowledge.  Normally used after the last byte of a multi-byte read.
//! @}

// Registers
#define LTC2944_STATUS_REG                          0x00
#define LTC2944_CONTROL_REG                         0x01
#define LTC2944_ACCUM_CHARGE_MSB_REG                0x02
#define LTC2944_ACCUM_CHARGE_LSB_REG                0x03
#define LTC2944_CHARGE_THRESH_HIGH_MSB_REG          0x04
#define LTC2944_CHARGE_THRESH_HIGH_LSB_REG          0x05
#define LTC2944_CHARGE_THRESH_LOW_MSB_REG           0x06
#define LTC2944_CHARGE_THRESH_LOW_LSB_REG           0x07
#define LTC2944_VOLTAGE_MSB_REG                     0x08
#define LTC2944_VOLTAGE_LSB_REG                     0x09
#define LTC2944_VOLTAGE_THRESH_HIGH_MSB_REG         0x0A
#define LTC2944_VOLTAGE_THRESH_HIGH_LSB_REG         0x0B
#define LTC2944_VOLTAGE_THRESH_LOW_MSB_REG          0x0C
#define LTC2944_VOLTAGE_THRESH_LOW_LSB_REG          0x0D
#define LTC2944_CURRENT_MSB_REG                     0x0E
#define LTC2944_CURRENT_LSB_REG                     0x0F
#define LTC2944_CURRENT_THRESH_HIGH_MSB_REG         0x10
#define LTC2944_CURRENT_THRESH_HIGH_LSB_REG         0x11
#define LTC2944_CURRENT_THRESH_LOW_MSB_REG          0x12
#define LTC2944_CURRENT_THRESH_LOW_LSB_REG          0x13
#define LTC2944_TEMPERATURE_MSB_REG                 0x14
#define LTC2944_TEMPERATURE_LSB_REG                 0x15
#define LTC2944_TEMPERATURE_THRESH_HIGH_REG         0x16
#define LTC2944_TEMPERATURE_THRESH_LOW_REG          0x17


// Command Codes
#define LTC2944_AUTOMATIC_MODE                     0xC0
#define LTC2944_SCAN_MODE                          0x80
#define LTC2944_MANUAL_MODE                     0x40
#define LTC2944_SLEEP_MODE                      0x00

#define LTC2944_PRESCALAR_M_1                   0x00
#define LTC2944_PRESCALAR_M_4                   0x08
#define LTC2944_PRESCALAR_M_16                  0x10
#define LTC2944_PRESCALAR_M_64                  0x18
#define LTC2944_PRESCALAR_M_256                 0x20
#define LTC2944_PRESCALAR_M_1024                0x28
#define LTC2944_PRESCALAR_M_4096                0x30
#define LTC2944_PRESCALAR_M_4096_2              0x31

#define LTC2944_ALERT_MODE                      0x04
#define LTC2944_CHARGE_COMPLETE_MODE            0x02

#define LTC2944_DISABLE_ALCC_PIN                0x00
#define LTC2944_SHUTDOWN_MODE                   0x01





void LTC2944_Init(void);
u8 WakeUp_AnalogPart(void);

int8_t LTC2944_read(uint8_t i2c_address, uint8_t adc_command);
int8_t LTC2944_read_16_bits(uint8_t i2c_address, uint8_t adc_command);
int8_t i2c_read_block_data(uint8_t address, uint8_t command, uint8_t length, uint8_t *values);

int8_t LTC2944_write(uint8_t i2c_address, uint8_t adc_command, uint8_t code);
int8_t LTC2944_write_16_bits(uint8_t i2c_address, uint8_t adc_command, uint16_t code);
#endif
