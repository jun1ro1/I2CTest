/* 
 * File:   i2c.h
 * Author: jun1ro1
 *
 * Created on March 6, 2016, 6:22 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define I2C_ACK        false
#define I2C_NACK       true
#define I2C_START      false
#define I2C_RESTART    true

void i2c_begin( void );

void i2c_wait_idle( void );

void i2c_write( const unsigned char data );
unsigned char i2c_read( const bool nack );

void i2c_begin_transmission( const unsigned char address, const bool restart );
void i2c_end_transmission( void );

void i2c_begin_request( const unsigned char address, const bool restart );
void i2c_end_request( void );

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

