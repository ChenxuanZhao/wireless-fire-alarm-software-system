/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#include "task.h"

#include <rtdevice.h>
#include <board.h>

#define DBG_TAG "task"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <sensor.h>
#include <iostream>

#include "server_interaction.h"
#include "smoke_sensor.h"
#include "easyblink.h"
#include "beep.h"
#include "key.h"
#include "lib_kalman.h"

/* defined the LED0 pin: PC13 */
#define LED0_PIN            GET_PIN(C, 13)
/* defined the BUZZER pin: PB0 */
#define BUZZER_PIN          GET_PIN(B, 0)

namespace
{
    rt_thread_t task_tid1 = nullptr;
    rt_thread_t task_tid2 = nullptr;
    bool alarm_flag = true;

    float g_temperature = 0;
    float g_concentration = 0;
    float g_temp_set = 60;
    float g_concentration_set = 2000;

    rt_mutex_t update_mutex = RT_NULL;

    void system_env_data_print()
    {
        rt_mutex_take(update_mutex, RT_WAITING_FOREVER);
        std::cout << std::endl;
        std::cout << "[data] Temperature: " << g_temperature << "℃, Concentration: " << g_concentration << "ppm"
                  << std::endl;
        std::cout << "[config] Temperature: " << g_temp_set << "℃, Concentration: " << g_concentration_set << "ppm"
                  << std::endl;
        std::cout << "[alarm] " << (alarm_flag ? "on" : "off") << std::endl;
        rt_mutex_release(update_mutex);
    }

    void key_event(uint8_t statue)
    {
        if (statue == 0)
        {
            if (task_tid1 != nullptr)
            {
                rt_thread_resume(task_tid1);
            }
        }
        else if (statue == 1)
        {
            alarm_flag = !alarm_flag;
            if (task_tid2 != nullptr)
            {
                rt_thread_resume(task_tid2);
            }
        }
        else if (statue == 2)
        {
            system_env_data_print();
        }
    }

    void task_01_handler(void *parameter)
    {
        float temp_set = 0;
        float concentration_set = 0;
        rt_thread_mdelay(10 * 1000);
        while (true)
        {
            float temp = 0;
            float concentration = 0;
            rt_mutex_take(update_mutex, RT_WAITING_FOREVER);
            temp = g_temperature;
            concentration = g_concentration;
            rt_mutex_release(update_mutex);
            
            if (server_config_update(&temp_set, &concentration_set) != RT_EOK)
            {
                LOG_D("Sync Failure!");
            }
            rt_mutex_take(update_mutex, RT_WAITING_FOREVER);
            if (200 <= concentration_set && concentration_set <= 10000)
            {
                g_temp_set = temp_set;
                g_concentration_set = concentration_set;
            }
            rt_mutex_release(update_mutex);
            std::cout << "[Upload] Temperature: " << temp << "℃, Concentration: " << concentration << "ppm"
                      << std::endl;
            if (server_data_update(temp, concentration) != RT_EOK)
            {
                LOG_D("Upload Fail!");
            }
            rt_thread_mdelay(10 * 1000);
        }
    }

    void task_02_handler(void *parameter)
    {
        struct rt_sensor_data sensor_data = {0};
        rt_device_t dev = rt_device_find("temp_lm75");

        if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
        {
            LOG_D("Open device failed!");
            return;
        }
        KalmanTypeDef kal_temp = {0};
        KalmanTypeDef kal_conc = {0};
        KalmanFilter_Init(&kal_temp);
        KalmanFilter_Init(&kal_conc);
        while (true)
        {
            float temp_lm75 = 0;
            float concentration = mq2_get_smokescope(MQ2_SPI_BUS_NAME);
            int res = rt_device_read(dev, 0, &sensor_data, 1);

            if (res != 1)
            {
                LOG_D("Read data failed! result is %d", res);
                rt_device_close(dev);
                return;
            }
            else
            {
                temp_lm75 = sensor_data.data.temp / 10;
            }
            rt_mutex_take(update_mutex, RT_WAITING_FOREVER);
            g_temperature = KalmanFilter_Update(&kal_temp, temp_lm75);
            g_concentration = KalmanFilter_Update(&kal_conc, concentration);
            if (alarm_flag && (g_temperature > g_temp_set || g_concentration > g_concentration_set))
            {
                beep(-1, 1000, 50, 0);
            }
            else
            {
                beep_stop();
            }
            rt_mutex_release(update_mutex);
            rt_thread_mdelay(1000);
        }
    }

    void task_init()
    {
        task_tid1 = rt_thread_create("task_01", task_01_handler, RT_NULL, 8 * 1024, 20, 20);
        if (task_tid1 != RT_NULL)
        {
            rt_thread_startup(task_tid1);
        }
        else
        {
            LOG_D("Task 01 start fail!");
        }
        task_tid2 = rt_thread_create("task_02", task_02_handler, RT_NULL, 1024, 19, 20);
        if (task_tid2 != RT_NULL)
        {
            rt_thread_startup(task_tid2);
        }
        else
        {
            LOG_D("Task 02 start fail!");
        }
        update_mutex = rt_mutex_create("update_mutex", RT_IPC_FLAG_FIFO);
        if (update_mutex == RT_NULL)
        {
            LOG_D("create update mutex failed.");
        }
    }

} // namespace

void task_application_start()
{
    ebled_t led0 = easyblink_init_led(LED0_PIN, PIN_LOW);
    easyblink(led0, -1, 500, 1000);
    beep_init(BUZZER_PIN, PIN_HIGH);
    task_init();
}

extern "C" void user_key_click_callback()
{
    key_event(0);
}

extern "C" void user_key_double_click_callback()
{
    key_event(1);
}

extern "C" void user_key_long_hold_callback()
{
    key_event(2);
}
