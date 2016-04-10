/* 
 * File:   lcd.h
 * Author: jun1ro1
 *
 * Created on March 20, 2016, 1:11 AM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "define.h"
#include <stdint.h>    
// I2C address
#define LCD_I2C_ADDRESS                 0x3E

#define LCD_COLS_MIN                       8
#define LCD_COLS_MAX                      40
#define LCD_ROWS_MIN                       1
#define LCD_ROWS_MAX                       2

#define LCD_CONTRAST_MAX                0x3F

#define LCD_COMMAND                     0x00
#define LCD_DATA                        0x40

#define LCD_CLEARDISPLAY                0x01
#define LCD_RETURNHOME                  0x02

#define LCD_ENTRYMODE                   0x04
#define LCD_ENTRYMODE_CURSORRIGHT       0x02

#define LCD_DISPLAY                     0x08
#define LCD_DISPLAY_DISPLAYON           0x04
#define LCD_DISPLAY_CURSORON            0x02
#define LCD_DISPLAY_CURSORBLINK         0x01

#define LCD_FUNCTION                    0x20
#define LCD_FUNCTION_8BITBUS            0x10
#define LCD_FUNCTION_2LINE              0x08
#define LCD_FUNCTION_DOUBLEHEIGHT       0x04
#define LCD_FUNCTION_EXTENSION          0x01

#define LCD_SETDDRAMADDRESS             0x80

  // extend instructions
#define LCD_EXT_INTERNALOSC             0x10
#define LCD_EXT_INTERNALOSC_ADJUST      0x04

#define LCD_EXT_POWER                   0x50
#define LCD_EXT_POWER_ICON              0x08
#define LCD_EXT_POWER_BOOSTER           0x04

#define LCD_EXT_FOLLOWER                0x60
#define LCD_EXT_FOLLOWER_ON             0x08
#define LCD_EXT_FOLLOWER_RAB2           0x04

#define LCD_EXT_CONTRAST                0x70

  void lcd_begin( const int cols, const int rows );
  void lcd_setContrast( const uint8_t contrast );

  void lcd_clear( void );
  void lcd_home( void );
  void lcd_setCursor( const int col, const int row );

  void lcd_cursor( void );
  void lcd_noCursor( void );
  void lcd_blink( void );
  void lcd_noBlink( void );
  void lcd_display( void );
  void lcd_noDisplay( void );

  void lcd_write( const char ch );
  void lcd_print( const int data, const int base );
  void lcd_printStr( const char *str );

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

