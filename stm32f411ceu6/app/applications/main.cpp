/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <iostream>

#include "task.h"

#define APP_VERSION "1.0.0"

int main(int argc, const char *argv[])
{
    LOG_D("The current version of APP firmware is %s", APP_VERSION);
    task_application_start();

    return RT_EOK;
}
