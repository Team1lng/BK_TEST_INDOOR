#ifndef CCTV_STREAM_POLICY_H
#define CCTV_STREAM_POLICY_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum
{
    CCTV_BRAND_DAHUA = 0,
    CCTV_BRAND_HIKVISION = 1,
    CCTV_BRAND_STECH = 2,
} cctv_brand_type;

typedef enum
{
    CCTV_STREAM_MAIN = 0,
    CCTV_STREAM_SUB = 1,
} cctv_stream_type;

/*
** 仅用于兼容旧配置：把老版本编码在 URL 里的码流选择解析出来。
** 码流选择的唯一来源已经是 user_data_info.camera_stream[]，不要再拿本函数当状态源。
** Stech 主/子码流现在共用同一条 URL，无法区分，这里只能认出老固件写下的 /video1/2。
*/
static inline cctv_stream_type cctv_stream_from_url(const char *url)
{
    if (url == NULL)
    {
        return CCTV_STREAM_MAIN;
    }

    // 子码流 URL 特征:
    //   HIKVISION: rtsp://.../Streaming/Channels/2
    //   STECH:     rtsp://.../video1/2   (旧格式，仅用于迁移)
    //   DAHUA:     rtsp://.../cam/realmonitor?channel=1&subtype=1
    if (strstr(url, "/Streaming/Channels/2") != NULL || strstr(url, "/video1/2") != NULL ||
        strstr(url, "subtype=1") != NULL)
    {
        return CCTV_STREAM_SUB;
    }

    return CCTV_STREAM_MAIN;
}

static inline bool cctv_stream_url_build(char *url, size_t url_size, int brand, cctv_stream_type stream,
                                         const char *account, const char *password, const char *ip)
{
    int written;

    if (url == NULL || url_size == 0 || account == NULL || password == NULL || ip == NULL)
    {
        return false;
    }

    if (brand == CCTV_BRAND_HIKVISION)
    {
        written = snprintf(url, url_size, "rtsp://%s:%s@%s:554/Streaming/Channels/%d",
                           account, password, ip, stream == CCTV_STREAM_SUB ? 2 : 1);
    }
    else if (brand == CCTV_BRAND_STECH)
    {
        // Stech 主码流和子码流用同一条地址(stream 不参与拼接): rtsp://账号:密码@IP:554/video1
        written = snprintf(url, url_size, "rtsp://%s:%s@%s:554/video1",
            account, password, ip);
    }
    else  // CCTV_BRAND_DAHUA
    {
        written = snprintf(url, url_size, "rtsp://%s:%s@%s:554/cam/realmonitor?channel=1&subtype=%d",
                           account, password, ip, stream == CCTV_STREAM_SUB ? 1 : 0);
    }

    return written >= 0 && (size_t)written < url_size;
}

#endif
