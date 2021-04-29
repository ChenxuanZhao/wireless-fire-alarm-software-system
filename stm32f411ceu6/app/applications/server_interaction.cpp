/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-01     Chenxuan     first version
 */

#include "server_interaction.h"

#define DBG_TAG "server_interaction"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <fstream>
#include <string>
#include "cJSON.h"

#include "http_webclient.h"

namespace
{
    int config_json_parse(const char *response_json, float *temp, float *concentration)
    {
        cJSON *root_json = nullptr;
        cJSON *item_json = nullptr;
        cJSON *results_json = nullptr;
        cJSON *tmp_json = nullptr;
        cJSON *smokescope_json = nullptr;
        int ret = 0;

        root_json = cJSON_Parse(response_json);
        if (root_json == nullptr)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != nullptr)
            {
                LOG_E("Error before: %s", error_ptr);
            }
            ret = -RT_ERROR;
            goto __exit;
        }

        item_json = cJSON_GetObjectItem(root_json, "results");
        results_json = cJSON_GetArrayItem(item_json, 0);
        tmp_json = cJSON_GetObjectItem(results_json, "temperatureThreshold");
        smokescope_json = cJSON_GetObjectItem(results_json, "concentrationThreshold");
        if (tmp_json != nullptr && smokescope_json != nullptr && temp != nullptr && concentration != nullptr)
        {
            *temp = tmp_json->valuedouble;
            *concentration = smokescope_json->valuedouble;
        }
        else
        {
            LOG_D("The data anomaly");
            ret = -RT_ERROR;
        }

    __exit:
        cJSON_Delete(root_json);

        return ret;
    }

    int sensor_data_download(float *temperature, float *smokescope)
    {
        std::string rece_buff;
        char *msg_json = nullptr;
        cJSON *root_json = nullptr;
        int ret = 0;

        if (webclient_get_comm(RESTFUL_API_CONFIG_URI, rece_buff) < 0)
        {
            LOG_E("Failed to connect to server");
            ret = -RT_ERROR;
            goto __exit;
        }

        /* Formatted data */
        root_json = cJSON_Parse(rece_buff.c_str());
        if (root_json == nullptr)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != nullptr)
            {
                LOG_E("Error before: %s", error_ptr);
            }
            ret = -RT_ERROR;
            goto __exit;
        }

        msg_json = cJSON_PrintUnformatted(root_json);
        if (msg_json == nullptr)
        {
            LOG_E("Insufficient memory");
            ret = -RT_ERROR;
            goto __exit;
        }

        config_json_parse(msg_json, temperature, smokescope);

    __exit:
        rt_free(msg_json);
        cJSON_Delete(root_json);

        return ret;
    }

    int sensor_data_upload(float temperature, float smokescope)
    {
        cJSON *root_json = cJSON_CreateObject();
        char *format_json = NULL;
        int ret = 0;
        std::string rece_buff;

        cJSON_AddItemToObject(root_json, "temperature", cJSON_CreateNumber(temperature));
        cJSON_AddItemToObject(root_json, "smokescope", cJSON_CreateNumber(smokescope));
        format_json = cJSON_PrintUnformatted(root_json);
        if (format_json == NULL)
        {
            LOG_D("The data anomaly");
            ret = -RT_ERROR;
            goto __exit;
        }

        if (webclient_post_comm(RESTFUL_API_DATA_URI, format_json, rt_strlen(format_json), rece_buff) < 0)
        {
            LOG_E("Failed to connect to server");
            ret = -RT_ERROR;
            goto __exit;
        }

    __exit:
        cJSON_Delete(root_json);
        rt_free(format_json);
        return ret;
    }
} // namespace

int server_config_update(float *temp, float *concentration)
{
    return sensor_data_download(temp, concentration);
}

int server_data_update(float temp, float concentration)
{
    return sensor_data_upload(temp, concentration);
}