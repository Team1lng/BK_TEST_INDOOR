#include <assert.h>

#include "tuya_session_guard.h"

int main(void)
{
    assert(!tuya_session_keep_shared_media(false, false));
    assert(tuya_session_keep_shared_media(true, false));
    assert(tuya_session_keep_shared_media(true, true));

    assert(!tuya_session_keep_shared_video(false));
    assert(tuya_session_keep_shared_video(true));

    assert(!tuya_session_local_monitor_allowed(true, false, false));
    assert(tuya_session_local_monitor_allowed(true, true, false));
    assert(!tuya_session_local_monitor_allowed(false, true, true));
    assert(tuya_session_local_monitor_allowed(false, true, false));

    return 0;
}
