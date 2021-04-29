/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#ifndef __SERVER_INTERACTION_H__
#define __SERVER_INTERACTION_H__

#include <rtthread.h>

#if defined(APP_RELEASES)
#define RESTFUL_API_DATA_URI            "http://172.20.10.2/api/v1/data/"
#define RESTFUL_API_CONFIG_URI          "http://172.20.10.2/api/v1/config/?ordering=-updateTime/"
#define CONFIG_FILE_NAME                "config.json"
#else
#define RESTFUL_API_DATA_URI            "https://design.xuanxuan.cf/api/v1/data/"
#define RESTFUL_API_CONFIG_URI          "https://design.xuanxuan.cf/api/v1/config/?ordering=-updateTime/"
#define CONFIG_FILE_NAME                "config.json"
#endif

int server_config_update(float *temp, float *concentration);
int server_data_update(float temp, float concentration);

#endif /* __SERVER_INTERACTION_H__ */
