#include <assert.h>
#include <string.h>

#include "../../api/common/cctv_stream_policy.h"

int main(void)
{
    char url[128] = {0};
    char sub_url[128] = {0};
    char round_url[128] = {0};
    const cctv_brand_type brands[2] = {CCTV_BRAND_DAHUA, CCTV_BRAND_HIKVISION};
    const cctv_stream_type streams[2] = {CCTV_STREAM_MAIN, CCTV_STREAM_SUB};
    int b;
    int s;

    assert(cctv_stream_from_url(NULL) == CCTV_STREAM_MAIN);
    assert(cctv_stream_from_url("") == CCTV_STREAM_MAIN);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/Streaming/Channels/1") == CCTV_STREAM_MAIN);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/Streaming/Channels/2") == CCTV_STREAM_SUB);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/cam/realmonitor?channel=1&subtype=0") == CCTV_STREAM_MAIN);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/cam/realmonitor?channel=1&subtype=1") == CCTV_STREAM_SUB);
    // 老固件把 STECH 子码流写成 /video1/2，仍要认得出来：user_data 的一次性迁移靠它
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/video1/2") == CCTV_STREAM_SUB);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/video1") == CCTV_STREAM_MAIN);

    assert(cctv_stream_url_build(url, sizeof(url), CCTV_BRAND_HIKVISION, CCTV_STREAM_MAIN, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/Streaming/Channels/1") == 0);

    assert(cctv_stream_url_build(url, sizeof(url), CCTV_BRAND_HIKVISION, CCTV_STREAM_SUB, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/Streaming/Channels/2") == 0);

    assert(cctv_stream_url_build(url, sizeof(url), CCTV_BRAND_DAHUA, CCTV_STREAM_MAIN, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/cam/realmonitor?channel=1&subtype=0") == 0);

    assert(cctv_stream_url_build(url, sizeof(url), CCTV_BRAND_DAHUA, CCTV_STREAM_SUB, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/cam/realmonitor?channel=1&subtype=1") == 0);

    // STECH 主/子码流共用同一条地址(实测只有这条能出图)
    assert(cctv_stream_url_build(url, sizeof(url), CCTV_BRAND_STECH, CCTV_STREAM_MAIN, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/video1") == 0);

    assert(cctv_stream_url_build(sub_url, sizeof(sub_url), CCTV_BRAND_STECH, CCTV_STREAM_SUB, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(sub_url, url) == 0);

    // 防回归核心：大华/海康 2 品牌 x 2 码流，build -> from_url 往返，回读值必须等于输入值。
    // STECH 不参与往返：它的地址故意不带码流信息，码流选择由 user_data_info.camera_stream[] 持久化。
    for (b = 0; b < 2; b++)
    {
        for (s = 0; s < 2; s++)
        {
            assert(cctv_stream_url_build(round_url, sizeof(round_url), brands[b], streams[s],
                                         "admin", "pwd", "192.168.1.2"));
            assert(cctv_stream_from_url(round_url) == streams[s]);
        }
    }

    return 0;
}
