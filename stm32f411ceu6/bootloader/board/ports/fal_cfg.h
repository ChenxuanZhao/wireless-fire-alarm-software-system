/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#ifndef __FAL_CFG_H__
#define __FAL_CFG_H__

#include <rtthread.h>
#include <board.h>

#define RT_APP_PART_ADDR            (0x08020000)       // app partition begin address

#define NOR_FLASH_DEV_NAME          FAL_USING_NOR_FLASH_DEV_NAME    // "norflash0"

#define FLASH_SIZE_GRANULARITY_16K   (4 *  16 * 1024)
#define FLASH_SIZE_GRANULARITY_64K   (1 *  64 * 1024)
#define FLASH_SIZE_GRANULARITY_128K  (3 * 128 * 1024)

#define STM32_FLASH_START_ADRESS_16K  STM32_FLASH_START_ADRESS
#define STM32_FLASH_START_ADRESS_64K  (STM32_FLASH_START_ADRESS_16K + FLASH_SIZE_GRANULARITY_16K)
#define STM32_FLASH_START_ADRESS_128K (STM32_FLASH_START_ADRESS_64K + FLASH_SIZE_GRANULARITY_64K)

extern const struct fal_flash_dev stm32_onchip_flash_16k;
extern const struct fal_flash_dev stm32_onchip_flash_64k;
extern const struct fal_flash_dev stm32_onchip_flash_128k;
extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &stm32_onchip_flash_16k,                                         \
    &stm32_onchip_flash_64k,                                         \
    &stm32_onchip_flash_128k,                                        \
    &nor_flash0,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG

/* partition table */
#define FAL_PART_TABLE                                                                           \
{                                                                                                \
    {FAL_PART_MAGIC_WROD, "bootloader", "onchip_flash_16k",  0, FLASH_SIZE_GRANULARITY_16K , 0}, \
    {FAL_PART_MAGIC_WROD, "param",      "onchip_flash_64k",  0, FLASH_SIZE_GRANULARITY_64K , 0}, \
    {FAL_PART_MAGIC_WROD, "app",        "onchip_flash_128k", 0, FLASH_SIZE_GRANULARITY_128K, 0}, \
    {FAL_PART_MAGIC_WORD, "factory",     NOR_FLASH_DEV_NAME, 0,                  512 * 1024, 0}, \
    {FAL_PART_MAGIC_WORD, "download",    NOR_FLASH_DEV_NAME,  512 * 1024,        512 * 1024, 0}, \
    {FAL_PART_MAGIC_WORD, "filesys",     NOR_FLASH_DEV_NAME, 1024 * 1024,  16 * 1024 * 1024, 0}, \
}

#endif /* FAL_PART_HAS_TABLE_CFG */
#endif /* __FAL_CFG_H__ */
