/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#include "smoke_sensor.h"

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define DBG_TAG "smoke_sensor"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#if defined(BSP_USING_ADS1118)

#include <math.h>
#include "ads1118.h"

#define MQ2_CAL_PPM 20
#define MQ2_RL 5

static float r0 = 0;

static float mq2_calibration(float rs)
{
    return rs / pow(MQ2_CAL_PPM / 613.9f, 1 / -2.074f);
}

static float mq2_get_voltage(const char *name)
{
    union ADS1118_CONFIG cfg = { 0 };
    cfg.field.SS = ADS1118_REG_SS_START;
    cfg.field.MUX = ADS1118_REG_MUX_AIN0_GND;
    cfg.field.PGA = ADS1118_REG_PGA_4096MV;
    cfg.field.MODE = ADS1118_REG_MODE_DEFAULT;
    cfg.field.DR = ADS1118_REG_DR_860SPS;
    cfg.field.TS_MODE = ADS1118_REG_TS_MODE_ADC;
    cfg.field.PULL_UP = ADS1118_REG_PULL_UP_DISABLE;
    cfg.field.NOP = ADS1118_REG_NOP_VALID;
    cfg.field.RESERVED = ADS1118_REG_RESERVED;
    return ads1118_read_data(name, cfg.reg);
}

float mq2_get_smokescope(const char *name)
{
    float voltage = mq2_get_voltage(name) + 0.01;
    const float rs = (3.3 - voltage) / voltage * MQ2_RL;

    return 613.9 * pow(rs / r0, -2.074);
}

static int rt_hw_mq2_init(void)
{
    for (int i = 0; i < 5; i++)
    {
        float voltage = mq2_get_voltage(MQ2_SPI_BUS_NAME);
        float rs = (3.3 - voltage) / voltage * MQ2_RL;
        r0 = mq2_calibration(rs);
    }
    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_mq2_init);
#endif
