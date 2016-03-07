
#include "i2c.h"

#define LCD_ADDRESS    0x3E

#define LCD_COMMAND    0x00

void lcd_begin( void ) {
  i2c_begin();
  __delay_ms( 40 );    // wait for 40 mil seconds after VDD stable

  lcd_send_command( 0x38 );  // Function Set
                             //  0 0 1 .  . . . .  Function Set
                             //  . . .DL  . . . .  1: 8-bit Bus Mode
                             //  . . . .  N . . .  1: 2-line Display Mode
                             //  . . . .  .DH . .  0: Double Height Font
                             //  . . . .  . . 0 .
                             //  . . . .  . . .IS  0 Normal Instruction Select

  lcd_send_command( 0x39 );  // function set
  lcd_send_command( 0x14 );  // internal OSC frequency
  lcd_send_command( 0x70 );  // contrast set
  lcd_send_command( 0x56 );  // Power/ICON/Contrast control
  lcd_send_command( 0x6C );  // Follwer control
  __delay_ms( 200 );

  lcd_send_command( 0x38 );  // function set
  lcd_send_command( 0x0C );  // display ON/OFF control
  lcd_clear();
}

void lcd_send_command( const unsigned char command ) {
  i2c_begin_transmission( LCD_ADDRESS, I2C_START );
  i2c_write( 0x00    );
  i2c_write( command );
  i2c_end_transmission();
  __delay_us( 27 );        // wait for 27 micro seconds
}

void lcd_clear( void ) {
  lcd_send_command( 0x01 );
  __dealy_us( 1080 - 27 );
}

void lcd_home( void ) {
  lcd_send_command( 0x02 );
}

void lcd_set_cursor( const unsigned char col, const unsigned char row ) {
}

void lcd_write( const char ch ) {

}

void lcd_print( const int data, const unsigned char base ) {
  char buf[ 8 ] = "12345678";
  itoa( data, buf, base );
  for( int i = 0; i < sizeof( buf ); i++ ) {
    lcd_write( buf[ i ] );
  }
}



