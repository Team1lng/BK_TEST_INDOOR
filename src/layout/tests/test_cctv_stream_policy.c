#include <assert.h>
#include <string.h>

#include "../../api/common/cctv_stream_policy.h"

int main(void)
{
    char url[128] = {0};

    assert(cctv_stream_from_url(NULL) == CCTV_STREAM_MAIN);
    assert(cctv_stream_from_url("") == CCTV_STREAM_MAIN);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/Streaming/Channels/1") == CCTV_STREAM_MAIN);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/Streaming/Channels/2") == CCTV_STREAM_SUB);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/cam/realmonitor?channel=1&subtype=0") == CCTV_STREAM_MAIN);
    assert(cctv_stream_from_url("rtsp://admin:pwd@192.168.1.2:554/cam/realmonitor?channel=1&subtype=1") == CCTV_STREAM_SUB);

    assert(cctv_stream_url_build(url, sizeof(url), true, CCTV_STREAM_MAIN, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/Streaming/Channels/1") == 0);

    assert(cctv_stream_url_build(url, sizeof(url), true, CCTV_STREAM_SUB, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/Streaming/Channels/2") == 0);

    assert(cctv_stream_url_build(url, sizeof(url), false, CCTV_STREAM_MAIN, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/cam/realmonitor?channel=1&subtype=0") == 0);

    assert(cctv_stream_url_build(url, sizeof(url), false, CCTV_STREAM_SUB, "admin", "pwd", "192.168.1.2"));
    assert(strcmp(url, "rtsp://admin:pwd@192.168.1.2:554/cam/realmonitor?channel=1&subtype=1") == 0);

    return 0;
}
