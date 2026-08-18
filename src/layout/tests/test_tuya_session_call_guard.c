#include <assert.h>
#include <stdbool.h>

extern bool tuya_session_indoor_call_blocked(bool local_video_client_active,
                                             bool family_monitor_active,
                                             bool local_monitor_entered_from_tuya,
                                             bool tuya_talk_active);
extern bool tuya_session_preserve_outdoor_tuya_order(bool local_video_client_active);

int main(void)
{
    assert(!tuya_session_indoor_call_blocked(false, false, false, false));
    assert(tuya_session_indoor_call_blocked(true, false, false, false));
    assert(!tuya_session_indoor_call_blocked(false, true, false, false));
    assert(tuya_session_indoor_call_blocked(false, false, true, false));
    assert(tuya_session_indoor_call_blocked(false, false, false, true));
    assert(!tuya_session_preserve_outdoor_tuya_order(false));
    assert(tuya_session_preserve_outdoor_tuya_order(true));

    return 0;
}
