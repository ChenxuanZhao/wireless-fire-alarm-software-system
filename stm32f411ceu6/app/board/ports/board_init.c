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

#include "fal.h"

#if defined(RT_USING_DFS)
#include "dfs_fs.h"
#endif

#define DBG_TAG "board_init"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define W25Q128_CS_PIN GET_PIN(A, 4)
#define ADS1118_CS_PIN GET_PIN(B, 1)

#define FS_PARTITION_NAME "filesys"

static int rt_board_spi_init(void)
{
    rt_pin_mode(W25Q128_CS_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(W25Q128_CS_PIN, PIN_HIGH);
    rt_pin_mode(ADS1118_CS_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(ADS1118_CS_PIN, PIN_HIGH);

    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_board_spi_init);

#if defined(APP_RELEASES) && !defined(PKG_USING_QBOOT)
/**
 * Function    ota_app_vtor_reconfig
 * Description Set Vector Table base location to the start addr of app(RT_APP_PART_ADDR).
*/
static int ota_app_vtor_reconfig(void)
{
#define NVIC_VTOR_MASK 0x3FFFFF80
    /* Set the Vector Table base location by user application firmware definition */
    SCB->VTOR = RT_APP_PART_ADDR & NVIC_VTOR_MASK;

    return 0;
}
INIT_BOARD_EXPORT(ota_app_vtor_reconfig);
#endif

static int rt_board_fal_init(void)
{
    fal_init();
#if defined(RT_USING_DFS)
#if defined(PKG_USING_LITTLEFS)
    /* 生成 mtd 设备 */
    struct rt_device *mtd_dev = fal_mtd_nor_device_create(FS_PARTITION_NAME);
    if (mtd_dev == RT_NULL)
    {
        LOG_E("Can't create a mtd device on '%s' partition.", FS_PARTITION_NAME);
    }
    else
    {
        /* 挂载 littlefs */
        if (dfs_mount(FS_PARTITION_NAME, "/", "lfs", 0, 0) == 0)
        {
            LOG_I("Filesystem initialized!");
        }
        else
        {
            /* 格式化文件系统 */
            dfs_mkfs("lfs", FS_PARTITION_NAME);
            /* 挂载 littlefs */
            if (dfs_mount("filesystem", "/", "lfs", 0, 0) == 0)
            {
                LOG_I("Filesystem initialized!");
            }
            else
            {
                LOG_E("Failed to initialize filesystem!");
            }
        }
    }
#elif defined(RT_USING_FATFS)
    struct rt_device *flash_dev = fal_blk_device_create(FS_PARTITION_NAME);

    if (flash_dev == RT_NULL)
    {
        LOG_D("Can't create a block device on '%s' partition.", FS_PARTITION_NAME);
    }
    else
    {
        LOG_D("Create a block device on the %s partition of flash successful.", FS_PARTITION_NAME);
    }

    if (rt_device_find(FS_PARTITION_NAME) != RT_NULL)
    {
        // dfs_mkfs("elm", FS_PARTITION_NAME);

        if (dfs_mount(FS_PARTITION_NAME, "/", "elm", 0, 0) == RT_EOK)
        {
            LOG_D("elm filesystem mount to '/'");
        }
        else
        {
            LOG_D("elm filesystem mount to '/' failed!");
        }
    }
    else
    {
        LOG_D("Find filesystem portion failed");
    }
#endif /* defined(PKG_USING_LITTLEFS) */
#endif /* defined(RT_USING_DFS) */

    return RT_EOK;
}
INIT_APP_EXPORT(rt_board_fal_init);
