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
static float mq2_get_voltage(const char *name)
{
    union ADS1118_CONFIG cfg = { 0 };
    cfg.field.SS = ADS1118_REG_SS_START;
    cfg.field.MUX = ADS1118_REG_MUX_AIN0_GND;
    cfg.field.PGA = ADS1118_REG_PGA_4096MV;
    cfg.field.MODE = ADS1118_REG_MODE_DEFAULT;
    cfg.field.DR = ADS1118_REG_DR_860SPS;
    cfg.field.TS_MODE = ADS1118_REG_TS_MODE_ADC;
    cfg.field.PULL_UP = ADS1118_REG_PULL_UP_ENABLE;
    cfg.field.NOP = ADS1118_REG_NOP_VALID;
    cfg.field.RESERVED = ADS1118_REG_RESERVED;
    ads1118_read_data(name, cfg.reg);
    ads1118_read_data(name, cfg.reg);
    rt_thread_mdelay(5);
    return ads1118_read_data(name, cfg.reg);
}

float mq2_get_smokescope(const char *name)
{
    float voltage = mq2_get_voltage(name);

    return voltage * (9800 / 3.3) + 200;
}
#endif
