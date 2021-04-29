#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX 8
#define RT_ALIGN_SIZE 4
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 256
#define RT_DEBUG
#define RT_DEBUG_COLOR

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_SMALL_MEM
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart1"
#define RT_VER_NUM 0x40002
#define ARCH_ARM
#define RT_USING_CPU_FFS
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 2048
#define RT_MAIN_THREAD_PRIORITY 10

/* C++ features */


/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_CMD_SIZE 80
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT
#define FINSH_ARG_MAX 10

/* Device virtual file system */


/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ 512
#define RT_USING_SERIAL
#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 64
#define RT_USING_PIN
#define RT_USING_SPI
#define RT_USING_SFUD
#define RT_SFUD_USING_SFDP
#define RT_SFUD_USING_FLASH_INFO_TABLE
#define RT_SFUD_SPI_MAX_HZ 50000000

/* Using USB */


/* POSIX layer and C standard library */

#define RT_LIBC_USING_TIME

/* Network */

/* Socket abstraction layer */


/* Network interface device */


/* light weight TCP/IP stack */


/* AT commands */


/* VBUS(Virtual Software BUS) */


/* Utilities */

#define RT_USING_RYM

/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */

#define PKG_USING_OTA_DOWNLOADER
#define PKG_USING_YMODEM_OTA
#define PKG_USING_OTA_DOWNLOADER_LATEST_VERSION

/* security packages */

#define PKG_USING_TINYCRYPT
#define PKG_USING_TINYCRYPT_LATEST_VERSION
#define TINY_CRYPT_MD5
#define TINY_CRYPT_BASE64
#define TINY_CRYPT_AES
#define TINY_CRYPT_AES_ROM_TABLES
#define TINY_CRYPT_SHA1
#define TINY_CRYPT_SHA256

/* language packages */


/* multimedia packages */


/* tools packages */


/* system packages */

#define PKG_USING_FAL
#define FAL_DEBUG 0
#define FAL_PART_HAS_TABLE_CFG
#define FAL_USING_SFUD_PORT
#define FAL_USING_NOR_FLASH_DEV_NAME "norflash0"
#define PKG_USING_FAL_LATEST_VERSION
#define PKG_FAL_VER_NUM 0x99999
#define PKG_USING_EV
#define PKG_USING_EV_LATEST_VERSION
#define PKG_USING_QBOOT
#define QBOOT_USING_PRODUCT_CODE
#define QBOOT_PRODUCT_CODE "rhICon6bzusKjFTs1LCJ"
#define QBOOT_APP_PART_NAME "app"
#define QBOOT_DOWNLOAD_PART_NAME "download"
#define QBOOT_FACTORY_PART_NAME "factory"
#define QBOOT_USING_AES
#define QBOOT_AES_IV "mjx8FCk3aON1mFkC"
#define QBOOT_AES_KEY "474QMsHYoqy0munR3GmrIoq4ik5bk21f"
#define QBOOT_USING_GZIP
#define QBOOT_USING_QUICKLZ
#define QBOOT_USING_FASTLZ
#define QBOOT_USING_SHELL
#define QBOOT_SHELL_KEY_CHK_TMO 5
#define QBOOT_USING_OTA_DOWNLOADER
#define QBOOT_USING_PRODUCT_INFO
#define QBOOT_PRODUCT_NAME "Graduation project"
#define QBOOT_PRODUCT_VER "v1.00 2020.10.01"
#define QBOOT_PRODUCT_MCU "STM32F411CEU6"
#define QBOOT_USING_STATUS_LED
#define QBOOT_STATUS_LED_PIN 45
#define QBOOT_STATUS_LED_LEVEL 1
#define QBOOT_USING_FACTORY_KEY
#define QBOOT_FACTORY_KEY_PIN 0
#define QBOOT_FACTORY_KEY_LEVEL 0
#define QBOOT_FACTORY_KEY_CHK_TMO 10
#define QBOOT_THREAD_STK_SIZE 4096
#define QBOOT_THREAD_PRIO 5
#define PKG_USING_QBOOT_LATEST_VERSION

/* peripheral libraries and drivers */

#define PKG_USING_QLED
#define QLED_TOTAL 5
#define QLED_TIME_UNIT_MS 10
#define QLED_THREAD_NAME "qled"
#define QLED_THREAD_STACK_SIZE 512
#define QLED_THREAD_PRIO 3
#define PKG_USING_QLED_LATEST_VERSION

/* miscellaneous packages */

#define PKG_USING_FASTLZ
#define PKG_USING_FASTLZ_LATEST_VERSION
#define PKG_USING_QUICKLZ
#define QLZ_COMPRESSION_LEVEL 3
#define PKG_USING_QUICKLZ_LATEST_VERSION
#define PKG_USING_ZLIB
#define PKG_USING_ZLIB_LATEST_VERSION

/* samples: kernel and components samples */

#define PKG_USING_CRCLIB
#define CRCLIB_USING_CRC8
#define CRC8_USING_CONST_TABLE
#define CRC8_POLY_8C
#define CRC8_POLY 0x8C
#define CRCLIB_USING_CRC16
#define CRC16_USING_CONST_TABLE
#define CRC16_POLY_A001
#define CRC16_POLY 0xA001
#define CRCLIB_USING_CRC32
#define CRC32_USING_CONST_TABLE
#define CRC32_POLY_EDB88320
#define CRC32_POLY 0xEDB88320
#define PKG_USING_CRCLIB_LATEST_VERSION
#define SOC_FAMILY_STM32
#define SOC_SERIES_STM32F4

/* Hardware Drivers Config */

#define SOC_STM32F411RE

/* Onboard Peripheral Drivers */

#define BSP_USING_SPI_FLASH

/* On-chip Peripheral Drivers */

#define BSP_USING_GPIO
#define BSP_USING_UART
#define BSP_USING_UART1
#define BSP_UART1_RX_USING_DMA
#define BSP_USING_SPI
#define BSP_USING_SPI1
#define BSP_SPI1_TX_USING_DMA
#define BSP_SPI1_RX_USING_DMA
#define BSP_USING_ON_CHIP_FLASH

/* Board extended module Drivers */


#endif
