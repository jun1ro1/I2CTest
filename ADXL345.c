#include <pic.h>
#include <xc.h>

#include "i2c.h"
#include "ADXL345.h"

// for DEBUG
#include "lcd.h"
extern void led(const int);


void ADXL345_begin( void ) {
  i2c_begin();
  ADXL345_writeRegister( ADXL345_REG_DATA_FORMAT,
                         ADXL345_REG_DATA_FORMAT_RANGE_2G ); // 0x31 <- 00:2G
  ADXL345_writeRegister( ADXL345_REG_FIFO_CTL, 0 ); // bypass mode

  // start to measure
  ADXL345_writeRegister( ADXL345_REG_POWER_CTL, ADXL345_REG_POWER_CTL_MEASURE );
  
  uint8_t id = ADXL345_readRegister( ADXL345_REG_DEVID );
  
  char str[] = "ID=";
  lcd_clear();
  lcd_home();
  lcd_printStr(str);
  lcd_print(id, 16);
  led(3);
  lcd_clear();
  lcd_home();
}

void ADXL345_writeRegister( uint8_t reg, uint8_t data ) {
  i2c_begin_transmission( ADXL345_I2C_ADDRESS, I2C_START );
  i2c_write( reg  );
  i2c_write( data );
  i2c_end_transmission();
}

uint8_t ADXL345_readRegister( uint8_t reg ) {
  // send a register address
  i2c_begin_transmission( ADXL345_I2C_ADDRESS, I2C_START );
  i2c_write( reg );

  // read a byte
  i2c_begin_request( ADXL345_I2C_ADDRESS, I2C_RESTART );
  uint8_t data = i2c_read( I2C_NACK ); // end of reading
  i2c_end_request();
  return data;
}

int16_t ADXL345_read16( uint8_t reg ) {
  // send a register address
  i2c_begin_transmission( ADXL345_I2C_ADDRESS, I2C_START );
  i2c_write( reg );

  // read a byte
  i2c_begin_request( ADXL345_I2C_ADDRESS, I2C_RESTART );
  uint16_t data = i2c_read( I2C_ACK  );  // read the lower byte
  data |= ( i2c_read( I2C_NACK ) << 8 ); // read the higher byte
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