#include <assert.h>

#include "../../tuya/tuya_video_frame_type.h"

int h264_is_keyframe(const unsigned char *buffer, int len)
{
    (void)len;
    return buffer[0] == 0x67 || buffer[0] == 0x65;
}

int main(void)
{
    const unsigned char sps_frame[] = {0, 0, 0, 0, 0x67};
    const unsigned char p_frame[] = {0, 0, 0, 0, 0x41};

    assert(tuya_h264_frame_type_get(sps_frame, sizeof(sps_frame)) == E_VIDEO_I_FRAME);
    assert(tuya_h264_frame_type_get(p_frame, sizeof(p_frame)) == E_VIDEO_PB_FRAME);

    return 0;
}
