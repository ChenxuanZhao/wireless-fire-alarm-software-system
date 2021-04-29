/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan    first version
 */

#include "key.h"

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define DBG_TAG "key"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#if defined(BSP_USING_USER_KEY)

#include "flexible_button.h"

#define USER_KEY0_PIN GET_PIN(A, 0)

#define ENUM_TO_STR(e) (#e)

typedef enum
{
    USER_BUTTON_0 = 0,
    USER_BUTTON_MAX
} user_button_t;

static char *enum_event_string[] = {
    ENUM_TO_STR(FLEX_BTN_PRESS_DOWN),
    ENUM_TO_STR(FLEX_BTN_PRESS_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_DOUBLE_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_REPEAT_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_MAX),
    ENUM_TO_STR(FLEX_BTN_PRESS_NONE),
};

static char *enum_btn_id_string[] = {
    ENUM_TO_STR(USER_BUTTON_0),
    ENUM_TO_STR(USER_BUTTON_1),
    ENUM_TO_STR(USER_BUTTON_2),
    ENUM_TO_STR(USER_BUTTON_3),
    ENUM_TO_STR(USER_BUTTON_MAX),
};

static flex_button_t user_button[USER_BUTTON_MAX] = { 0 };

static uint8_t common_btn_read(void *arg)
{
    uint8_t value = 0;

    flex_button_t *btn = (flex_button_t *)arg;

    switch (btn->id)
    {
    case USER_BUTTON_0:
        value = rt_pin_read(USER_KEY0_PIN);
        break;
    default:
        break;
    }

    return value;
}

RT_WEAK void user_key_click_callback(void)
{
    LOG_D("[user_key]: FLEX_BTN_PRESS_CLICK");
}

RT_WEAK void user_key_double_click_callback(void)
{
    LOG_D("[user_key]: FLEX_BTN_PRESS_DOUBLE_CLICK");
}

RT_WEAK void user_key_long_hold_callback(void)
{
    LOG_D("[user_key]: FLEX_BTN_PRESS_LONG_HOLD");
}

static void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;

    // LOG_D("id: [%d - %s]  event: [%d - %30s]  repeat: %d", btn->id, enum_btn_id_string[btn->id], btn->event, enum_event_string[btn->event],btn->click_cnt);

    if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_CLICK)
    {
        LOG_D("[User Key] BTN PRESS CLICK");
        user_key_click_callback();
    }
    else if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_DOUBLE_CLICK)
    {
        LOG_D("[User Key] BTN PRESS DOUBLE CLICK");
        user_key_double_click_callback();
    }
    else if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_LONG_HOLD)
    {
        LOG_D("[User Key] BTN PRESS LONGHOLD");
        user_key_long_hold_callback();
    }
}

static void button_scan(void *arg)
{
    while (1)
    {
        flex_button_scan();
        rt_thread_mdelay(20); // 20 ms
    }
}

static void user_button_init(void)
{
    rt_pin_mode(USER_KEY0_PIN, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */

    for (int i = USER_BUTTON_0; i < USER_BUTTON_MAX; ++i)
    {
        user_button[i].id = i;
        user_button[i].usr_button_read = common_btn_read;
        user_button[i].cb = common_btn_evt_cb;
        user_button[i].pressed_logic_level = 0;
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500);
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(3000);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(4500);

        flex_button_register(&user_button[i]);
    }
}

static int flex_button_main(void)
{
    rt_thread_t tid = RT_NULL;

    user_button_init();

    /* Create background ticks thread */
    tid = rt_thread_create("flex_btn", button_scan, RT_NULL, 1024, 10, 10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return 0;
}
INIT_APP_EXPORT(flex_button_main);
#endif
