/*********************************************************************************
 *Copyright(C),2015-2020,
 *TUYA
 *FileName: tuya_ipc_dp_.c
 *
 * File Description：
 * 1. API implementation of DP point
 *
 * This file code is the basic code, users don't care it
 * Please do not modify any contents of this file at will.
 * Please contact the Product Manager if you need to modify it.
 *
 **********************************************************************************/
#include "utilities/uni_log.h"
#include "tuya_iot_config.h"
#include "tuya_ipc_api.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_iot_config.h"
#include "ty_dp_define.h"
#include "ty_sdk_common.h"
#include "utilities/uni_log.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TUYA_UNPACK_ENABLE 0
#if defined(TUYA_UNPACK_ENABLE) && (TUYA_UNPACK_ENABLE == 1)
#include "tuya_unpack.h"

ota_stream *ota_obj = NULL;
#endif

/* OTA */
// Callback after downloading OTA files
OPERATE_RET __IPC_APP_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    FILE *p_upgrade_fd = (FILE *)pri_data;
    if (p_upgrade_fd)
    {
        fclose(p_upgrade_fd);
    }

    printf("Upgrade Finish\n\r");
    printf("download_result:%d fw_url:%s\n\r", download_result, fw->fw_url);

    if (download_result == 0)
    {
        /* The developer needs to implement the operation of OTA upgrade,
        when the OTA file has been downloaded successfully to the specified path. [ p_mgr_info->upgrade_file_path ]*/
    }
    if (fw->tp == DEV_NM_ATH_SNGL) // 室内机升级包，开始升级室内机
    {
        extern int ak_drv_wdt_close(void);
        ak_drv_wdt_close();
        system("cp /etc/config/wpa_supplicant.conf /app/data");
        system("tar -zxvf /tmp/TWO_WIRE_APP -C /tmp;killall daemon.sh;killall app.sh;killall ANYKA37E.BIN;/tmp/update.sh &");
    }
    else // 门口机升级包，开始升级门口机
    {
        system("mv /tmp/TWO_WIRE_APP /tmp/cbin.update");
        extern bool tuya_ota_upgrade_event(void);
        tuya_ota_upgrade_event();
    }

    // TODO
    // reboot system
    return OPRT_OK;
}

// To collect OTA files in fragments and write them to local files
OPERATE_RET __IPC_APP_get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                       IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    // printf("Rev File Data:\n\r");
    // printf("total_len:%d  fw_url:%s\n\r", total_len, fw->fw_url);
    // printf("Offset:%d Len:%d\n\r", offset, len);

    // report UPGRADE process, NOT only download percent, consider flash-write time
    // APP will report overtime fail, if uprgade process is not updated within 60 seconds

    int download_percent = (offset * 100) / (total_len + 1);
    int report_percent = download_percent; // as an example, download 100% = 50%  upgrade work finished

    tuya_ipc_upgrade_progress_report_by_type(report_percent, fw->tp);
    // tuya_ipc_upgrade_progress_report(report_percent);

    FILE *p_upgrade_fd = (FILE *)pri_data;
    if (p_upgrade_fd)
    {
        fwrite(data, 1, len, p_upgrade_fd);
    }
    // APP will report "uprage success" after reboot and new FW version is reported inside SDK automaticlly

    return OPRT_OK;
}

INT_T TUYA_IPC_Upgrade_Inform_cb(IN CONST FW_UG_S *fw)
{
    printf("Rev Upgrade Info \n\r");
    printf("fw->fw_url:%s \n\r", fw->fw_url);
    printf("fw->fw_md5:%s \n\r", fw->fw_md5);
    printf("fw->sw_ver:%s \n\r", fw->sw_ver);
    printf("fw->file_size:%u \n\r", fw->file_size);
    printf("fw->fw_hmac:%s \n\r", fw->fw_hmac);
    FILE *p_upgrade_fd = fopen("/tmp/TWO_WIRE_APP", "w+b");
    tuya_ipc_upgrade_sdk(fw, __IPC_APP_get_file_data_cb, __IPC_APP_upgrade_notify_cb, p_upgrade_fd);

    return OPRT_OK;
}