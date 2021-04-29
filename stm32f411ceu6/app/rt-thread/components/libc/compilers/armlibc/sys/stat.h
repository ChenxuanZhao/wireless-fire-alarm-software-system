/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#ifndef __STAT_H__
#define __STAT_H__

#ifdef __cplusplus
extern "C" {
#endif

int stat(const char *pathname, struct stat *statbuf);
int fstat(int fd, struct stat *statbuf);
int lstat(const char *pathname, struct stat *statbuf);

#ifdef __cplusplus
}
#endif

#endif /* __STAT_H__ */
