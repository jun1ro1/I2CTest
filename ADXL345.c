#include <pic.h>
#include <xc.h>

#include "i2c.h"
#include "ADXL345.h"

void ADXL345_begin( void ) {
  i2c_begin();
  ADXL345_writeRegister( ADXL345_REG_POWER_CTL, 0x08 ); // Measure bit on
  ADXL345_writeRegister( ADXL345_REG_DATA_FORMAT,
                         ADXL345_REG_DATA_FORMAT_RANGE_2G ); // 2G
}

void ADXL345_writeRegister( uint8_t reg, uint8_t data ) {
  i2c_begin_transmission( ADXL345_I2C_ADDRESS, I2C_START );
  i2c_write( reg  );
  i2c_write( data );
  i2c_end_transmission();
}

uint8_t ADXL345_readRegister( uint8_t reg ) {
  // send a register
  i2c_begin_transmission( ADXL345_I2C_ADDRESS, I2C_START );
  i2c_write( reg );
  i2c_end_transmission();

  // read a byte
  i2c_begin_request( ADXL345_I2C_ADDRESS, I2C_RESTART );
  uint8_t data = i2c_read( I2C_NACK );
  i2c_end_request();
  return data;
}

int16_t ADXL345_read16( uint8_t reg ) {
  // send a register number
  i2c_begin_transmission( ADXL345_I2C_ADDRESS, I2C_START );
  i2c_write( reg );
  i2c_end_transmission();

  // read a byte
  i2c_begin_request( ADXL345_I2C_ADDRESS, I2C_RESTART );
  uint16_t data = i2c_read( I2C_ACK  );  // read the lower byte
  data |= ( i2c_read( I2C_NACK ) << 8 ); // read the higer byte
  i2c_end_request();
  return (int16_t)data;
}

int16_t ADXL345_getX( void ) {
  return ADXL345_read16( ADXL345_REG_DATAX0 );
}

int16_t ADXL345_getY( void ) {
  return ADXL345_read16( ADXL345_REG_DATAY0 );
}

int16_t ADXL345_getZ( void ) {
  return ADXL345_read16( ADXL345_REG_DATAZ0 );
}
