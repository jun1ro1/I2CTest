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

#define I2C_ACK        0
#define I2C_NACK       1
#define I2C_STAT       0
#define I2C_RESTART    1

void i2c_begin( void);

void i2c_wait_idle( void );

void i2c_write( const unsigned char data );
unsigned char i2c_read( const unsigned char nack );

void i2c_begin_transmission( const unsigned char address, const unsigned char restart );
void i2c_end_transmission( void );

void i2c_begin_request( const unsigned char address, const unsigned char restart );
void i2c_end_request( void );


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

