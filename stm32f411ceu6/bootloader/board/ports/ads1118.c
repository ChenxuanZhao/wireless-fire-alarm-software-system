/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#include "ads1118.h"

#define DRV_DEBUG
#define LOG_TAG              "ads1118"

#include <drv_log.h>

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_spi.h>

#if defined(BSP_USING_LM75)

float ads1118_read_data(const char *name, const uint16_t cmd)
{
    uint16_t recv_buf = 0;
    union ADS1118_CONFIG cfg = { 0 };
    struct rt_spi_device *dev = (struct rt_spi_device *)rt_device_find(name);
    if (dev == RT_NULL)
    {
        LOG_D("Can't find %s device!", ADS1118_SPI_DEVICE_NAME);
        return 0;
    }
    else
    {
        rt_spi_transfer(dev, &cmd, &recv_buf, 1);
        cfg.reg = cmd;
        if (cfg.field.TS_MODE == ADS1118_REG_TS_MODE_TEMP)
        {
            float temperature = 0;
            recv_buf >>= 2;
            if (recv_buf & 0x2000)
            {
                recv_buf = ~recv_buf + 1;
                recv_buf &= 0x7FF;
                temperature = -recv_buf * 0.03125;
            }
            else
            {
                temperature = recv_buf * 0.03125;
            }
            return temperature;
        }
        else
        {
            float voltage = recv_buf * 1.0 / 32768 * 4.096;
            if (voltage < 0)
            {
                voltage = 0;
            }
            else if (voltage > 3.3)
            {
                voltage = 3.3;
            }
            return voltage;
        }
    }
}

static int ads1118_hw_init(void)
{
    rt_hw_spi_device_attach(ADS1118_SPI_BUS, ADS1118_SPI_DEVICE_NAME, GPIOB, GPIO_PIN_1);
    struct rt_spi_device *dev = (struct rt_spi_device *)rt_device_find(ADS1118_SPI_DEVICE_NAME);
    if (dev == RT_NULL)
    {
        LOG_D("Can't find %s device!", ADS1118_SPI_DEVICE_NAME);
        return -RT_ERROR;
    }
    else
    {
        // Configure
        struct rt_spi_configuration cfg = { 0 };

        cfg.data_width = 16;
        cfg.max_hz = 30 * 1000 * 1000;      // 30M
        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_1 | RT_SPI_MSB;

        rt_spi_configure(dev, &cfg);
    }
    return RT_EOK;
}

INIT_DEVICE_EXPORT(ads1118_hw_init);

#endif
