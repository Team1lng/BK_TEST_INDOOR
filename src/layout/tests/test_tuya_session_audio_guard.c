#include <assert.h>

#include "tuya_session_guard.h"

int main(void)
{
    assert(!tuya_session_keep_shared_media(false, false));
    assert(tuya_session_keep_shared_media(true, false));
    assert(tuya_session_keep_shared_media(true, true));

    assert(!tuya_session_keep_shared_video(false));
    assert(tuya_session_keep_shared_video(true));

    assert(tuya_session_local_monitor_allowed(true, false, false));
    assert(tuya_session_local_monitor_allowed(true, true, false));
    assert(!tuya_session_local_monitor_allowed(false, true, true));
    assert(tuya_session_local_monitor_allowed(false, true, false));

    assert(tuya_session_local_talk_allowed(false, false, false));
    assert(!tuya_session_local_talk_allowed(true, false, false));
    assert(!tuya_session_local_talk_allowed(false, true, false));
    assert(!tuya_session_local_talk_allowed(false, false, true));

    assert(tuya_session_local_talk_button_allowed(true, true, true, true));
    assert(tuya_session_local_talk_button_allowed(false, false, false, false));
    assert(!tuya_session_local_talk_button_allowed(false, true, false, false));
    assert(!tuya_session_local_talk_button_allowed(false, false, true, false));
    assert(!tuya_session_local_talk_button_allowed(false, false, false, true));

    return 0;
}
