/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#ifndef __ADS1118_H__
#define __ADS1118_H__

#include <rtthread.h>

/* SPI DEVICE */
#define ADS1118_SPI_BUS            "spi1"
#define ADS1118_SPI_DEVICE_NAME    "spi11"

/* Config Register */

#define ADS1118_REG_SS_START            (1)

#define ADS1118_REG_MUX_AIN0_AIN1       (0b000)
#define ADS1118_REG_MUX_AIN0_AIN3       (0b001)
#define ADS1118_REG_MUX_AIN1_AIN3       (0b010)
#define ADS1118_REG_MUX_AIN2_AIN3       (0b011)
#define ADS1118_REG_MUX_AIN0_GND        (0b100)
#define ADS1118_REG_MUX_AIN1_GND        (0b101)
#define ADS1118_REG_MUX_AIN2_GND        (0b110)
#define ADS1118_REG_MUX_AIN3_GND        (0b111)

#define ADS1118_REG_PGA_6144MV          (0b000)
#define ADS1118_REG_PGA_4096MV          (0b001)
#define ADS1118_REG_PGA_2048MV          (0b010)
#define ADS1118_REG_PGA_1024MV          (0b011)
#define ADS1118_REG_PGA_0512MV          (0b100)
#define ADS1118_REG_PGA_0256MV          (0b101)

#define ADS1118_REG_MODE_DEFAULT        (1)

#define ADS1118_REG_DR_8SPS             (0b000)
#define ADS1118_REG_DR_16SPS            (0b001)
#define ADS1118_REG_DR_32SPS            (0b010)
#define ADS1118_REG_DR_64SPS            (0b011)
#define ADS1118_REG_DR_128SPS           (0b100)
#define ADS1118_REG_DR_250SPS           (0b101)
#define ADS1118_REG_DR_475SPS           (0b110)
#define ADS1118_REG_DR_860SPS           (0b111)

#define ADS1118_REG_TS_MODE_ADC         (0)
#define ADS1118_REG_TS_MODE_TEMP        (1)

#define ADS1118_REG_PULL_UP_DISABLE     (0)
#define ADS1118_REG_PULL_UP_ENABLE      (1)

#define ADS1118_REG_NOP_VALID           (0b01)

#define ADS1118_REG_RESERVED            (1)

union ADS1118_CONFIG
{
    uint16_t reg;
    struct __field
    {
        uint8_t RESERVED   : 1;
        uint8_t NOP        : 2;
        uint8_t PULL_UP    : 1;
        uint8_t TS_MODE    : 1;
        uint8_t DR         : 3;
        uint8_t MODE       : 1;
        uint8_t PGA        : 3;
        uint8_t MUX        : 3;
        uint8_t SS         : 1;
    } field;
};

#ifdef __cplusplus
extern "C" {
#endif

float ads1118_read_data(const char *name,const uint16_t cmd);

#ifdef __cplusplus
}
#endif

#endif /* __ADS1118_H__ */
