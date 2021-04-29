/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#ifndef __SMOKE_SENSOR_H__
#define __SMOKE_SENSOR_H__

#define MQ2_SPI_BUS_NAME "spi10"

#ifdef __cplusplus
extern "C" {
#endif

float mq2_get_smokescope(const char *name);

#ifdef __cplusplus
}
#endif

#endif /* __SMOKE_SENSOR_H__ */
