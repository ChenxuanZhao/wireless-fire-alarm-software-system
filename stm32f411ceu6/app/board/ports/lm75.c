/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan    first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#define DBG_TAG "lm75"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define SENSOR_TEMP_RANGE_MAX (125)
#define SENSOR_TEMP_RANGE_MIN (-55)

#define LM75_I2C_BUS_NAME          "i2c1"  /* 传感器连接的I2C总线设备名称 */
#define LM75_ADDR                  0x48    /* 从机地址 */

#if defined(BSP_USING_LM75)

#include <sensor.h>
static float lm75_get_temperature(const char *name, uint16_t addr)
{
    uint8_t buf[2] = { 0 };
    uint16_t tmp = 0;
    float temperature = 0;
    struct rt_i2c_bus_device *i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(name);

    if (i2c_bus == RT_NULL)
    {
        LOG_D("Can't find %s device!", name);
        return 0;
    }
    else
    {
        rt_i2c_master_recv(i2c_bus, addr, 0, buf, 2);
    }

    tmp = (buf[0] * 0x100 + buf[1]) >> 5;
    if (tmp & 0x400)
    {
        tmp = ~tmp + 1;
        tmp &= 0x7FF;
        temperature = -tmp * 0.125;
    }
    else
    {
        temperature = tmp * 0.125;
    }

    return temperature;
}

static rt_size_t _lm75_polling_get_data(rt_sensor_t sensor, struct rt_sensor_data *data)
{
    if (sensor->info.type == RT_SENSOR_CLASS_TEMP)
    {
        float temperature_x10 =
            10 * lm75_get_temperature(sensor->config.intf.dev_name, (rt_uint32_t)sensor->config.intf.user_data);
        data->data.temp = (rt_int32_t)temperature_x10;
        data->timestamp = rt_sensor_get_ts();
    }
    return 1;
}

static rt_size_t lm75_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    RT_ASSERT(buf);

    if (sensor->config.mode == RT_SENSOR_MODE_POLLING)
    {
        return _lm75_polling_get_data(sensor, buf);
    }
    else
    {
        return 0;
    }
}

static rt_err_t lm75_control(struct rt_sensor_device *sensor, int cmd, void *args)
{
    rt_err_t result = RT_EOK;

    return result;
}

static struct rt_sensor_ops sensor_ops =
    {
        lm75_fetch_data,
        lm75_control
    };

static int rt_hw_lm75_init(const char *name, struct rt_sensor_config *cfg)
{
    rt_int8_t result;
    rt_sensor_t sensor = RT_NULL;

    sensor = rt_calloc(1, sizeof(struct rt_sensor_device));
    if (sensor == RT_NULL)
    {
        return -RT_ENOMEM;
    }

    sensor->info.type = RT_SENSOR_CLASS_TEMP;
    sensor->info.vendor = RT_SENSOR_VENDOR_UNKNOWN;
    sensor->info.model = "lm75_temp";
    sensor->info.unit = RT_SENSOR_UNIT_DCELSIUS;
    sensor->info.intf_type = RT_SENSOR_INTF_I2C;
    sensor->info.range_max = SENSOR_TEMP_RANGE_MAX;
    sensor->info.range_min = SENSOR_TEMP_RANGE_MIN;
    sensor->info.period_min = 100;

    rt_memcpy(&sensor->config, cfg, sizeof(struct rt_sensor_config));
    sensor->ops = &sensor_ops;

    result = rt_hw_sensor_register(sensor, name, RT_DEVICE_FLAG_RDONLY | RT_DEVICE_FLAG_FIFO_RX, RT_NULL);
    if (result != RT_EOK)
    {
        LOG_E("device register err code: %d", result);
        rt_free(sensor);
        return -RT_ERROR;
    }

    LOG_I("temp sensor init success");
    return RT_EOK;
}

static int rt_hw_lm75_port(void)
{
    struct rt_sensor_config cfg;
    cfg.intf.dev_name = LM75_I2C_BUS_NAME;
    cfg.intf.user_data = (void *)LM75_ADDR;
    rt_hw_lm75_init("lm75", &cfg);

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_lm75_port);

#endif
