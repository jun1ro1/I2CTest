#include <PIC.h>
#include <XC.h>
#include "i2c.h"

// #define DEBUG 1

static uint8_t _i2c_initialized = 0;

// extern void led(const int);

void i2c_begin(void) {
    if (!_i2c_initialized) {
        LATA |= 0x06; // RA1(SCL), RA2(SDA) : default higth
        TRISA |= 0x06; // RA1(SCL), RA2(SDA) : input
        ANSELA &= ~0x06; // RA1(SCL), RA2(SDA) : digital I/O

        SSP1STAT = 0; // clear SSP1STAT
        SSP1CON1 = 0x28; // SSP1EN : Enable SDA, SCL; SSP1M : 0b1000 I2C master mode
        SSP1CON2 = 0; // ACKEN, RCEN, PEN, RSEN, SEN : 0
        SSP1ADD = 0x13; // SCK clock 100kHz when CPU clock 8MHz
        _i2c_initialized = 1;
    }
}

void i2c_wait_idle(void) {
    // SSP1CON2.ACKEN RCEN PEN RSEN SEN
#ifndef DEBUG    
    while ((SSP1CON2 & 0x1F) || (SSP1STATbits.R_nW));
    //    led(2);
#endif
}

void i2c_write(const uint8_t data) {
    SSP1BUF = data;
    //    led(1);
    while (SSP1STATbits.BF); // wait until Buffer Full
    //    while (SSP1CON2bits.ACKSTAT); // wait for ACK received
    //    led(2);
    i2c_wait_idle();
    //    led(2);
}

uint8_t i2c_read(const bool nack) {
    SSP1CON2bits.RCEN = 1; // Receive Eanble
#ifndef DEBUG
    while (!SSP1STATbits.BF); // wait for SSP1BUF full
#endif
    uint8_t data = SSP1BUF;
    SSP1CON2bits.ACKDT = (nack) ? 1 : 0; // send NAK or ACK
    SSP1CON2bits.ACKEN = 1; // send ACK
    SSP1CON1bits.SSPOV = 0; // clear Receive Overflow
    i2c_wait_idle();
    return data;
}

void i2c_begin_transmission(const uint8_t address, const bool restart) {
    i2c_wait_idle();
    if (restart) {
        SSP1CON2bits.RSEN = 1; // send restart condition
#ifndef DEBUG
        while (SSP1CON2bits.RSEN); // wait to send restart condition
#endif        
    } else {
        SSP1CON2bits.SEN = 1; // send start 
#ifndef DEBUG        
        while (SSP1CON2bits.SEN); // wait to send start condition
#endif        
    }
    i2c_write(address << 1);
}

void i2c_end_transmission(void) {
    //    led(3);
    SSP1CON2bits.PEN = 1; // send stop condition
#ifndef DEBUG
    while (SSP1CON2bits.PEN); // wait to send start condition
#endif
    //    led(4);
}

void i2c_begin_request(const uint8_t address, const bool restart) {
    i2c_wait_idle();
    if (restart) {
        SSP1CON2bits.RSEN = 1; // send restart condition
#ifndef DEBUG        
        while (SSP1CON2bits.RSEN); // wait to send restart condition
#endif
    } else {
        SSP1CON2bits.SEN = 1; // send start condition
#ifndef DEBUG
        while (SSP1CON2bits.SEN); // wait to send start condition
#endif
    }

    // #pragma warning push
#pragma warning disable 752
    i2c_write(address << 1 | 0x01); // read mode
#pragma warning enable  752
    // #pragma warning pop
}

void i2c_end_request(void) {
    SSP1CON2bits.PEN = 1; // send stop condition
#ifndef DEBUG
    while (SSP1CON2bits.PEN); // wait to send start condition
#endif
}
