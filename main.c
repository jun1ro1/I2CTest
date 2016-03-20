/* 
 * File:   main.c
 * Author: jun1ro1
 *
 * Created on March 6, 2016, 6:12 PM
 */

#define _XTAL_FREQ 8000000

#include <stdio.h>
#include <stdlib.h>

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
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
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

int main(int argc, char** argv) {

    i2c_begin();
    
    lcd_begin( 8, 2 );
    const char str1[] = "Hello";
    const char str2[] = "World";
    
    lcd_home();
    lcd_setCursor(0,0);
    lcd_printStr( str1, sizeof( str1 ) );
    lcd_setCursor(0,1);
    lcd_printStr( str2, sizeof( str2 ) );
    
    while(1);
    
    return (EXIT_SUCCESS);
}

