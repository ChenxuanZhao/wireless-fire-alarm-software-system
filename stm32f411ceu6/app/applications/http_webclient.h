/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#ifndef __HTTP_WEBCLIENT_H__
#define __HTTP_WEBCLIENT_H__

#include <rtthread.h>
#include <webclient.h>

#include <string>

int webclient_get_comm(const char *uri, std::string &response);
int webclient_post_comm(const char *uri, const char *post_data, size_t data_len, std::string &response);

#endif /* __HTTP_WEBCLIENT_H__ */
