#include <PIC.h>
#include "i2c.h"

static unsigned char _i2c_initialized = 0;

void i2c_begin(void) {
    if (!_i2c_initialized) {
        LATA |= 0x06; // RA1(SCL), RA2(SDA) : default higth
        TRISA |= 0x06; // RA1(SCL), RA2(SDA) : input
        ANSEL &= ~0x06; // RA1(SCL), RA2(SDA) : digital I/O

        SSP1STAT = 0; // clear SSP1STAT
        SSP1CON1 = 0x28; // SSP1EN : Enable SDA, SCL; SSP1M : 0b1000 I2C master mode
        SSP1CON2 = 0; // ACKEN, RCEN, PEN, RSEN, SEN : 0
        SSPADD = 0x13; // SCK clock 100kHz when CPU clock 8MHz
        _i2c_initialized = 1;
    }
}

void i2c_wait_idle(void) {
    // SSP1CON2.ACKEN RCEN PEN RSEN SEN
    while ((SSP1CON2 & 0x1F) || (SSP1STATbits.R_nW));
}

void i2c_write(const unsigned char data) {
    SSP1BUF = data;
    while (SSP1STATbits.BF); // wait until Buffer Full
    while (SSP1CON2bits.ACKSTAT); // wait for ACK received
    i2c_wait_idle();
}

unsigned char i2c_read(const unsigned char nack) {
    SSP1CON2bits.RCEN = 1; // Receive Eanble
    while (!SSP1STATbits.BF); // wait for SSP1BUF full
    unsigned char data = SSP1BUF;
    SSP1CON2bits.ACKDT = (nack) ? 1 : 0; // send NAK or ACK
    SSP1CON2bits.ACKEN = 1; // send ACK
    SSP1CON1bits.SSPOV = 0; // clear Receive Overflow
    i2c_wait_idle();
    return data;
}

void i2c_begin_transmission(const unsigned char address, const unsigned char restart) {
    i2c_wait_idle();
    if (restart) {
        SSP1CON2bits.RSEN = 1; // send restart condition
        while (SSP1CON2bits.RSEN); // wait to send restart condition
    } else {
        SSP1CON2bits.SEN = 1; // send start condition
        while (SSP1CON2bits.SEN); // wait to send start condition
    }
    i2c_write(address << 1);
}

void i2c_end_transmission(void) {
    SSP1CON2bits.PEN = 1; // send stop condition
    while (SSP1CON2bits.PEN); // wait to send start condition
}

void i2c_begin_request(const unsigned char address, const unsigned char restart) {
    i2c_wait_idle();
    if (restart) {
        SSP1CON2bits.RSEN = 1; // send restart condition
        while (SSP1CON2bits.RSEN); // wait to send restart condition
    } else {
        SSP1CON2bits.SEN = 1; // send start condition
        while (SSP1CON2bits.SEN); // wait to send start condition
    }
    i2c_write(address << 1 | 0x01); // read mode
}

void i2c_end_request(void) {
    SSP1CON2bits.PEN = 1; // send stop condition
    while (SSP1CON2bits.PEN); // wait to send start condition
}


