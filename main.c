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


static volatile unsigned long _ticks = 0;

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

    __delay_ms(500);

    // timer1 interrupt
    T1CON = 0x8d;
    TMR1H = 0xC0;
    TMR1L = 0;

    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE  = 1;

    while (1) {
        lcd_clear();
        lcd_home();

        const char label[] = "Z=";
        lcd_printStr(label);
        int accel = ADXL345_getZ();
        lcd_print(accel, 10);

        lcd_setCursor(0,1);
        lcd_print(_ticks, 10);

        SLEEP();
        NOP();

        //        __delay_ms(500);
        //        led(1);
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
