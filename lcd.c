#include <pic.h>
#include <xc.h>
#include <stdlib.h>

#include "i2c.h"
#include "lcd.h"

// definitions
static int _lcd_cols = 0;
static int _lcd_rows = 0;

// register save
static uint8_t _lcd_entrymode = 0;
static uint8_t _lcd_display   = 0;
static uint8_t _lcd_function  = 0;
static uint8_t _lcd_power     = 0;

// internal functions


void lcd_send_command(const uint8_t command) {
    i2c_begin_transmission(LCD_I2C_ADDRESS, I2C_START);
    i2c_write(LCD_COMMAND);
    i2c_write(command);
    i2c_end_transmission();
    __delay_us(27); // wait for 27 micro seconds
}

void lcd_instruction_extension(void) {
    _lcd_function |= LCD_FUNCTION_EXTENSION;
    lcd_send_command(_lcd_function);
}

void lcd_instruction_normal(void) {
    _lcd_function &= ~LCD_FUNCTION_EXTENSION;
    lcd_send_command(_lcd_function);
}

void lcd_begin(const int cols, const int rows) {
    i2c_begin();
    __delay_ms(40); // wait for 40 mil seconds after VDD stable

    _lcd_cols = cols < LCD_COLS_MIN ? LCD_COLS_MIN :
            (cols > LCD_COLS_MAX ? LCD_COLS_MAX : cols);
    _lcd_rows = (rows < LCD_ROWS_MIN) ? LCD_ROWS_MIN :
            ((rows > LCD_ROWS_MAX) ? LCD_ROWS_MAX : rows);

    // clear register save
    _lcd_entrymode = 0;
    _lcd_display   = LCD_DISPLAY;
    _lcd_function  = LCD_FUNCTION;
    _lcd_power     = LCD_EXT_POWER;

    _lcd_function |= (LCD_FUNCTION_8BITBUS | LCD_FUNCTION_2LINE);
    lcd_send_command(_lcd_function); // 0x38

    lcd_instruction_extension(); // 0x39

    lcd_send_command(LCD_EXT_INTERNALOSC | LCD_EXT_INTERNALOSC_ADJUST); // 0x14

    // contrast = 0x02 << 4 | 0x00 = 0x20
    _lcd_power |= (LCD_EXT_POWER_BOOSTER | 0x02);
    lcd_send_command(LCD_EXT_CONTRAST | 0x00); // 0x70
    lcd_send_command(_lcd_power); // 0x56

    lcd_send_command(LCD_EXT_FOLLOWER | LCD_EXT_FOLLOWER_ON | LCD_EXT_FOLLOWER_RAB2); // 0x6C
    __delay_ms(200);

    lcd_instruction_normal(); // 0x38

    lcd_display(); // 0x0C
    lcd_clear();   // 0x01
}

void lcd_clear(void) {
    lcd_send_command(LCD_CLEARDISPLAY);
    __delay_us(1080 - 27);
}

void lcd_home(void) {
    lcd_send_command(LCD_RETURNHOME);
    __delay_us(1080 - 27);
}

void lcd_setCursor(const int col, const int row) {
    int cl = col < 0 ? 0 : (col >= _lcd_cols ? _lcd_cols - 1 : col);
    int rw = row < 0 ? 0 : (row >= _lcd_rows ? _lcd_rows - 1 : row);   
    uint8_t addr = cl + 0x40 * rw;
    lcd_send_command(LCD_SETDDRAMADDRESS | (addr & 0x7F));
}

void lcd_write(const char ch) {
    i2c_begin_transmission(LCD_I2C_ADDRESS, I2C_START);
    i2c_write(LCD_DATA);
    i2c_write(ch);
    i2c_end_transmission();
    __delay_us(27); // wait for 27 micro seconds
}

void lcd_cursor(void) {
    _lcd_display |= LCD_DISPLAY_CURSORON;
    lcd_send_command(_lcd_display);
}

void lcd_noCursor(void) {
    _lcd_display &= ~LCD_DISPLAY_CURSORON;
    lcd_send_command(_lcd_display);
}

void lcd_blink(void) {
    _lcd_display |= LCD_DISPLAY_CURSORBLINK;
    lcd_send_command(_lcd_display);
}

void lcd_noBlink(void) {
    _lcd_display &= ~LCD_DISPLAY_CURSORBLINK;
    lcd_send_command(_lcd_display);
}

void lcd_display(void) {
    _lcd_display |= LCD_DISPLAY_DISPLAYON;
    lcd_send_command(_lcd_display);
}

void lcd_noDisplay(void) {
    _lcd_display &= ~LCD_DISPLAY_DISPLAYON;
    lcd_send_command(_lcd_display);
}

void lcd_print(const int data, const int base) {
    char buf[ 17 ] = "****************"; // 16bits + EOS
    itoa(buf, data, base);
    lcd_printStr(buf);
}

void lcd_printStr(const char *str) {
    int i = 0;
    char ch;
    while( (ch = str[i]) != '\0') {
        lcd_write(ch);
        ++i;
    }
}

void lcd_setContrast(const uint8_t contrast) {
    uint8_t ct = contrast & LCD_CONTRAST_MAX;
    lcd_instruction_extension();
    lcd_send_command(LCD_EXT_CONTRAST | (ct & 0x0F));
    _lcd_power |= (ct >> 4);
    lcd_send_command(_lcd_power);
    lcd_instruction_normal();
}
