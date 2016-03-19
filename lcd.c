#include <pic.h>
#include <xc.h>
#include <stdlib.h>

#include "i2c.h"
#include "lcd.h"

// definitions
static int _lcd_cols = 0;
static int _lcd_rows = 0;

// register save
static unsigned char _lcd_entrymode = 0;
static unsigned char _lcd_display   = 0;
static unsigned char _lcd_function  = 0;
static unsigned char _lcd_power     = 0;

// internal functions
void lcd_send_command( const unsigned char command ) {
  i2c_begin_transmission( LCD_I2C_ADDRESS, I2C_START );
  i2c_write( LCD_COMMAND );
  i2c_write( command     );
  i2c_end_transmission();
  __delay_us( 27 );        // wait for 27 micro seconds
}

void lcd_instruction_extension( void ) {
  _lcd_function |= LCD_FUNCTION_EXTENSION;
  lcd_send_command( _lcd_function );
}

void lcd_instruction_normal( void ) {
  _lcd_function &= ~LCD_FUNCTION_EXTENSION;
  lcd_send_command( _lcd_function );
}


void lcd_begin( const int cols, const int rows ) {
  i2c_begin();
  __delay_ms( 40 );    // wait for 40 mil seconds after VDD stable

  int cl = cols;
  cl = ( cl < LCD_COLS_MIN ) ? LCD_COLS_MIN : cl;
  cl = ( cl > LCD_COLS_MAX ) ? LCD_COLS_MAX : cl;
  _lcd_cols = cl;

  int rw = rows;
  rw = ( rw < LCD_ROWS_MIN ) ? LCD_ROWS_MIN : rw;
  rw = ( rw > LCD_ROWS_MAX ) ? LCD_ROWS_MAX : rw;
  _lcd_rows = rw;
  
  // clear register save
  _lcd_entrymode = 0;
  _lcd_display   = 0;
  _lcd_function  = 0;
  _lcd_power     = 0;

  _lcd_function |= ( LCD_FUNCTION | LCD_FUNCTION_8BITBUS | LCD_FUNCTION_2LINE );
  lcd_send_command( _lcd_function );                                    // 0x38

  lcd_instruction_extension();                                          // 0x39
  lcd_send_command( LCD_EXT_INTERNALOSC | LCD_EXT_INTERNALOSC_ADJUST ); // 0x14

  // contrast = 0x02 << 4 | 0x00 = 0x20
  lcd_send_command( LCD_EXT_CONTRAST | 0x00 );                          // 0x70
  _lcd_power |= ( LCD_EXT_POWER | LCD_EXT_POWER_BOOSTER | 0x02 );
  lcd_send_command( _lcd_power );                                       // 0x5E

  lcd_send_command( LCD_EXT_FOLLOWER | LCD_EXT_FOLLOWER_ON | LCD_EXT_FOLLOWER_RAB2 ); // 0x6A
  __delay_ms( 200 );

  lcd_instruction_normal();

  lcd_clear();

  /* lcd_send_command( 0x38 );  // Function Set */
  /*                            //  0 0 1 .  . . . .  Function Set */
  /*                            //  . . .DL  . . . .  1: 8-bit Bus Mode */
  /*                            //  . . . .  N . . .  1: 2-line Display Mode */
  /*                            //  . . . .  .DH . .  0: Double Height Font */
  /*                            //  . . . .  . . 0 . */
  /*                            //  . . . .  . . .IS  0 Normal Instruction Select */

  /* lcd_send_command( 0x39 );  // function set */
  /* lcd_send_command( 0x14 );  // internal OSC frequency */
  /* lcd_send_command( 0x70 );  // contrast set */
  /* lcd_send_command( 0x56 );  // Power/ICON/Contrast control */
  /* lcd_send_command( 0x6C );  // Follwer control */
  /* __delay_ms( 200 ); */

  /* lcd_send_command( 0x38 );  // function set */
  /* lcd_send_command( 0x0C );  // display ON/OFF control */
  /* lcd_clear(); */
}

void lcd_clear( void ) {
  lcd_send_command( LCD_CLEARDISPLAY );
  __delay_us( 1080 - 27 );
}

void lcd_home( void ) {
  lcd_send_command( LCD_RETURNHOME );
  __delay_us( 1080 - 27 );
}

void lcd_setCursor( const int col, const int row ) {
  int cl = col;
  cl = ( cl < 0          ) ?              0 : cl;
  cl = ( cl >= _lcd_cols ) ?  _lcd_cols - 1 : cl;

  int rw = row;
  rw = ( rw < 0          ) ?              0 : rw;
  rw = ( rw >= _lcd_rows ) ?  _lcd_rows - 1 : rw;

  unsigned char addr = cl + 0x40 * ( rw - 1 );
  lcd_send_command( LCD_SETDDRAMADDRESS | ( addr & 0x7F ) );
}

void lcd_write( const unsigned char ch ) {
  i2c_begin_transmission( LCD_I2C_ADDRESS, I2C_START );
  i2c_write( LCD_DATA );
  i2c_write( ch       );
  i2c_end_transmission();
  __delay_us( 27 );        // wait for 27 micro seconds
}

void lcd_cursor( void ) {
  _lcd_display |= LCD_DISPLAY_CURSORON;
  lcd_send_command( _lcd_display );
}

void lcd_noCursor( void ) {
  _lcd_display &= ~LCD_DISPLAY_CURSORON;
  lcd_send_command( _lcd_display );
}

void lcd_blink( void ) {
  _lcd_display |= LCD_DISPLAY_CURSORBLINK;
  lcd_send_command( _lcd_display );
}

void lcd_noBlink( void ) {
  _lcd_display &= ~LCD_DISPLAY_CURSORBLINK;
  lcd_send_command( _lcd_display );
}

void lcd_display( void ) {
  _lcd_display |= LCD_DISPLAY_DISPLAYON;
  lcd_send_command( _lcd_display );
}

void lcd_noDisplay( void ) {
  _lcd_display &= ~LCD_DISPLAY_DISPLAYON;
  lcd_send_command( _lcd_display );
}


void lcd_print( const int data, const int base ) {
  char buf[ 10 ] = "*********";
  itoa( data, buf, base );
  for( int i = 0; i < sizeof( buf ); i++ ) {
    lcd_write( buf[ i ] );
  }
}

void lcd_printStr( const char *str, const int length ) {
  for( int i = 0; i < length; i++ ) {
    lcd_write( str[ i ] );
  }
}


void lcd_setContrast( const unsigned int contrast ) {
  unsigned char ct = contrast;
  ct &= LCD_CONTRAST_MAX;
  lcd_send_command( LCD_EXT_CONTRAST | ( ct & 0x0F ) );
  _lcd_power |= ( ct >> 4 );
  lcd_send_command( _lcd_power );
}
