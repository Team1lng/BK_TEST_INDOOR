#ifndef CCTV_STREAM_POLICY_H
#define CCTV_STREAM_POLICY_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum
{
    CCTV_STREAM_MAIN = 0,
    CCTV_STREAM_SUB = 1,
} cctv_stream_type;

static inline cctv_stream_type cctv_stream_from_url(const char *url)
{
    if (url == NULL)
    {
        return CCTV_STREAM_MAIN;
    }

    if (strstr(url, "/Streaming/Channels/2") != NULL || strstr(url, "subtype=1") != NULL)
    {
        return CCTV_STREAM_SUB;
    }

    return CCTV_STREAM_MAIN;
}

static inline bool cctv_stream_url_build(char *url, size_t url_size, bool hikvision, cctv_stream_type stream,
                                         const char *account, const char *password, const char *ip)
{
    int written;

    if (url == NULL || url_size == 0 || account == NULL || password == NULL || ip == NULL)
    {
        return false;
    }

    if (hikvision)
    {
        written = snprintf(url, url_size, "rtsp://%s:%s@%s:554/Streaming/Channels/%d",
                           account, password, ip, stream == CCTV_STREAM_SUB ? 2 : 1);
    }
    else
    {
        written = snprintf(url, url_size, "rtsp://%s:%s@%s:554/cam/realmonitor?channel=1&subtype=%d",
                           account, password, ip, stream == CCTV_STREAM_SUB ? 1 : 0);
    }

    return written >= 0 && (size_t)written < url_size;
}

#endif
