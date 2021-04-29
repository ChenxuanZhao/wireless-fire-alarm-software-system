/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#include "http_webclient.h"

#define DBG_TAG "http_webclient"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define GET_HEADER_BUFSZ               2048
#define GET_RESP_BUFSZ                 2048
#define POST_HEADER_BUFSZ              2048
#define POST_RESP_BUFSZ                2048

int webclient_get_comm(const char *uri, std::string &response)
{
    struct webclient_session *session = nullptr;
    unsigned char *buffer = nullptr;
    int ret = 0;
    int bytes_read, resp_status;
    int content_length = -1;

    response.clear();

    buffer = (unsigned char *)web_calloc(GET_RESP_BUFSZ, sizeof(unsigned char));
    if (buffer == nullptr)
    {
        LOG_D("No memory for receive buffer.");
        ret = -RT_ENOMEM;
        goto __exit;

    }

    /* Create webclient session and set header response size. */
    session = webclient_session_create(GET_HEADER_BUFSZ);
    if (session == nullptr)
    {
        ret = -RT_ENOMEM;
        goto __exit;
    }

    /* Send GET request by default header. */
    if ((resp_status = webclient_get(session, uri)) != 200)
    {
        LOG_D("Webclient GET request failed, response(%d) error.", resp_status);
        ret = -RT_ERROR;
        goto __exit;
    }

    content_length = webclient_content_length_get(session);
    if (content_length < 0)
    {
        do
        {
            bytes_read = webclient_read(session, (void *)buffer, GET_RESP_BUFSZ);
            if (bytes_read <= 0)
            {
                break;
            }

            for (int index = 0; index < bytes_read; index++)
            {
                response += buffer[index];
            }
        } while (true);
    }
    else
    {
        int content_pos = 0;

        do
        {
            bytes_read = webclient_read(session, (void *)buffer,
                content_length - content_pos > GET_RESP_BUFSZ ?
                GET_RESP_BUFSZ : content_length - content_pos);
            if (bytes_read <= 0)
            {
                break;
            }

            for (int index = 0; index < bytes_read; index++)
            {
                response += buffer[index];
            }

            content_pos += bytes_read;
        } while (content_pos < content_length);
    }

__exit:
    if (session)
    {
        webclient_close(session);
    }

    if (buffer)
    {
        web_free(buffer);
    }

    return ret;
}

int webclient_post_comm(const char *uri, const char *post_data, size_t data_len, std::string &response)
{
    struct webclient_session *session = nullptr;
    unsigned char *buffer = nullptr;
    int ret = 0;
    int bytes_read, resp_status;

    buffer = (unsigned char *)web_calloc(POST_RESP_BUFSZ, sizeof(unsigned char));
    if (buffer == nullptr)
    {
        LOG_D("No memory for receive response buffer.");
        ret = -RT_ENOMEM;
        goto __exit;
    }

    /* Create webclient session and set header response size. */
    session = webclient_session_create(POST_HEADER_BUFSZ);
    if (session == nullptr)
    {
        ret = -RT_ENOMEM;
        goto __exit;
    }

    /* Build header for upload. */
    webclient_header_fields_add(session, "Content-Length: %d\r\n", data_len);
    webclient_header_fields_add(session, "Content-Type: application/json\r\n");

    /* Send POST request by default header. */
    if ((resp_status = webclient_post(session, uri, post_data, data_len)) != 201)
    {
        LOG_D("Webclient POST request failed, response(%d) error.", resp_status);
        ret = -RT_ERROR;
        goto __exit;
    }
    do
    {
        bytes_read = webclient_read(session, buffer, POST_RESP_BUFSZ);
        if (bytes_read <= 0)
        {
            break;
        }

        for (int index = 0; index < bytes_read; index++)
        {
            response += buffer[index];
        }
    } while (true);
__exit:
    if (session)
    {
        webclient_close(session);
    }

    if (buffer)
    {
        web_free(buffer);
    }

    return ret;
}
