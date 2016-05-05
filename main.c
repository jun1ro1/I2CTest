/*
 * File:   main.c
 * Author: jun1ro1
 *
 * Created on March 6, 2016, 6:12 PM
 */

#include "define.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 *
 */

// PIC12F1822 Configuration Bit Settings

// 'C' source line config statements

#include <pic.h>
#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
// CONFIG1

// PIC12F1822 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

#include "i2c.h"
#include "lcd.h"
#include "ADXL345.h"

/* void led(const int count) { */
/*     for (int i = 0; i < count; i++) { */
/*         RA4 = 1; // LED on */
/*         __delay_ms(150); */
/*         RA4 = 0; // LED off */
/*         __delay_ms(150); */
/*     } */
/*     __delay_ms(150); */
/* } */


// --------------------------------------------------

#include <time.h>
static volatile long _ticks = 0;

time_t ticks_time( void ) {
  return _ticks >> 1;
}

typedef struct ElapsedTimer {
  time_t _origin;
}  ElapsedTimer_t;

void ElapsedTimer_create( ElapsedTimer_t *timer ) {
  timer->_origin = (time_t)0;
}

void ElapsedTimer_start( ElapsedTimer_t *timer ) {
  timer->_origin = ticks_time();
}

void ElapsedTimer_stop( ElapsedTimer_t *timer ){
  timer->_origin = (time_t)0;
}

long ElapsedTimer_elapsed( ElapsedTimer_t *timer ){
  if (timer->_origin ==(time_t)0) {
    return (time_t)0;
  }
  else {
    return ticks_time() - timer->_origin;
  }
}


typedef enum {
    tmMillis,  tmSecond, tmMinute, tmHour, tmWday, tmDay,tmMonth, tmYear, tmNbrFields
} tmByteFields;


tmByteFields roundTime( const long time, uint8_t *val ) {
    if (time >= (48UL * 60UL * 60UL)) {
        // rather than 2 days
        *val = time / (24UL * 60UL * 60UL); // seconds to days
        return tmDay;
    }
    else if (time >= 60 * 60) {
        *val = time / ( 60 * 60 );    // seconds to hours
        return tmHour;
    }
    else if (time >= 60) {
        *val = time / 60;             // seconds to minutes
        return tmMinute;
    }
    else {
        *val = time;
        return tmSecond;
    }
}

// --------------------------------------------------

int main(int argc, char** argv) {
    OSCCON = 0b01110010; // clock 8MHz internal clock
    ANSELA = 0; // all registers are digital
    TRISA  = 0; // all registers are output

//    led(1);
    i2c_begin();

    //    led(2);
    lcd_begin(8, 2);

//    led(3);
    ADXL345_begin();

//    led(4);

    const char str1[] = "Hello";
    const char str2[] = "World";

//    lcd_setContrast( 70 );
//      lcd_display();

    lcd_home();
    lcd_setCursor(0, 0);
    lcd_printStr(str1);
    lcd_setCursor(1, 1);
    lcd_printStr(str2);

    //    led(5);

    ElapsedTimer_t timer;
    ElapsedTimer_create( &timer );

    __delay_ms(500);

    // timer1 interrupt
    T1CON = 0x8d;
    TMR1H = 0xC0;
    TMR1L = 0;

    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE  = 1;

    bool display = true;
    lcd_display();
    ElapsedTimer_start(&timer);

    while (1) {
        int x = ADXL345_getX();
        int y = ADXL345_getY();

        if ((x > 100) || (x < -100) || (y > 100) || (y < -100)) {
            display = true;
            lcd_display();
            ElapsedTimer_start(&timer);
        } else if (ElapsedTimer_elapsed(&timer) > 15) {
            display = false;
            lcd_noDisplay();
            ElapsedTimer_stop(&timer);
        } else if (display) {
            lcd_home();
            lcd_clear();

            lcd_printStr("Z=");
            lcd_print(ADXL345_getZ(), 10);

            lcd_setCursor(0, 1);

            uint8_t        val = 0;
            tmByteFields field = roundTime( ticks_time(), &val );

            switch (field) {
            case tmSecond:  // Second
              lcd_printStr("S:");
              break;
            case tmMinute:  // minute
              lcd_printStr("M:");
              break;
            case tmHour:    // hour
              lcd_printStr("H:");
              break;
            case tmDay:     // day
              lcd_printStr("D:");
              break;
            default:
              break;
            }
            lcd_print(val, 10);

            //  long t = ticks_time();
            //  lcd_print(t, 10);

        }

        SLEEP();
        NOP();
   }

    return (EXIT_SUCCESS);
}


void interrupt isr( void ) {
  if ( PIR1bits.TMR1IF ) {
    PIR1bits.TMR1IF = 0;
    TMR1H = 0xc0;
    _ticks++;
  }
}




