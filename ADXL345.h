/* 
 * File:   ADXL345.h
 * Author: jun1ro1
 *
 * Created on March 21, 2016, 5:48 PM
 */

#ifndef ADXL345_H
#define	ADXL345_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "define.h"
#include <stdint.h>

#define ADXL345_I2C_ADDRESS         0x1C

#define ADXL345_REG_DEVID           0x00
#define ADXL345_REG_THRESH_TAP      0x1D
#define ADXL345_REG_OFSX            0x1E
#define ADXL345_REG_OFSY            0x1F
#define ADXL345_REG_OFSZ            0x20
#define ADXL345_REG_DUR             0x21
#define ADXL345_REG_LATENT          0x22
#define ADXL345_REG_WINDOW          0x23
#define ADXL345_REG_THRESH_ACT      0x24
#define ADXL345_REG_THRESH_INACT    0x25
#define ADXL345_REG_TIME_INACT      0x26
#define ADXL345_REG_ACT_INACT_CTL   0x27
#define ADXL345_REG_THRESH_FF       0x28
#define ADXL345_REG_TIME_FF         0x29
#define ADXL345_REG_TAP_AXES        0x2A
#define ADXL345_REG_ACT_TAP_STATUS  0x2B
#define ADXL345_REG_BW_RATE         0x2C
#define ADXL345_REG_POWER_CTL       0x2D
#define ADXL345_REG_INT_ENAGBLE     0x23
#define ADXL345_REG_INT_MAP         0x2F
#define ADXL345_REG_INT_SOURCE      0x30
#define ADXL345_REG_DATA_FORMAT     0x31
#define ADXL345_REG_DATAX0          0x32
#define ADXL345_REG_DATAX1          0x33
#define ADXL345_REG_DATAY0          0x34
#define ADXL345_REG_DATAY1          0x35
#define ADXL345_REG_DATAZ0          0x36
#define ADXL345_REG_DATAZ1          0x37
#define ADXL345_REG_FIFO_CTL        0x38
#define ADXL345_REG_FIFO_STATUS     0x39


#define ADXL345_REG_POWER_CTL_LINK          0x20
#define ADXL345_REG_POWER_CTL_AUTO_SLEEP    0x10
#define ADXL345_REG_POWER_CTL_MEASURE       0x08
#define ADXL345_REG_POWER_CTL_SLEEP         0x04

#define ADXL345_REG_DATA_FORMAT_SELF_TEST   0x80
#define ADXL345_REG_DATA_FORMAT_SPI         0x40
#define ADXL345_REG_DATA_FORMAT_INT_INVERT  0x20
#define ADXL345_REG_DATA_FORMAT_FULL_RES    0x08
#define ADXL345_REG_DATA_FORMAT_JUSTIFY     0x04
#define ADXL345_REG_DATA_FORMAT_RANGE_2G    0x00
#define ADXL345_REG_DATA_FORMAT_RANGE_4G    0x01
#define ADXL345_REG_DATA_FORMAT_RANGE_8G    0x02
#define ADXL345_REG_DATA_FORMAT_RANGE_16G   0x03


    void ADXL345_begin(void);

    void ADXL345_writeRegister(uint8_t reg, uint8_t data);
    uint8_t ADXL345_readRegister(uint8_t reg);
    int16_t ADXL345_read16(uint8_t reg);

    int16_t ADXL345_getX(void);
    int16_t ADXL345_getY(void);
    int16_t ADXL345_getZ(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ADXL345_H */
