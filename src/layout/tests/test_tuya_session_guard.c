#include <assert.h>

#include "tuya_session_guard.h"

int main(void)
{
    assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_HOME));
    assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_STANDBY));
    assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_TIME_DISPLAY));

    assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_MEDIA));
    assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_WIFI));
    assert(!tuya_session_target_allowed(TUYA_SESSION_TARGET_UPGRADE));
    assert(!tuya_session_target_allowed(TUYA_SESSION_TARGET_MONITOR));
    assert(!tuya_session_target_allowed(TUYA_SESSION_TARGET_CCTV));
    assert(!tuya_session_target_allowed(TUYA_SESSION_TARGET_INTERPHONE));
    assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_TRANSFER));
    assert(tuya_session_target_allowed(TUYA_SESSION_TARGET_OTHER));

    assert(tuya_session_should_ignore_indoor_busy(true));
    assert(!tuya_session_should_ignore_indoor_busy(false));
    assert(!tuya_session_remote_busy_should_interrupt_ui());
    assert(tuya_session_remote_channel_switch_allowed(false));
    assert(!tuya_session_remote_channel_switch_allowed(true));

    return 0;
}
