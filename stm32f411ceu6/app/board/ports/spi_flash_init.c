/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     Add spi flash port file
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define DBG_TAG "spi_flash_init"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "spi_flash.h"
#include "spi_flash_sfud.h"
#include "drv_spi.h"

#define W25Q128_SPI_BUS_NAME       "spi1"
#define W25Q128_SPI_DEVICE_NAME    "spi10"

#if defined(BSP_USING_SPI_FLASH)
static int rt_hw_spi_flash_init(void)
{
    rt_hw_spi_device_attach(W25Q128_SPI_BUS_NAME, W25Q128_SPI_DEVICE_NAME, GPIOA, GPIO_PIN_4);

    if (rt_sfud_flash_probe("W25Q128", W25Q128_SPI_DEVICE_NAME) == RT_NULL)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_init);
#endif
