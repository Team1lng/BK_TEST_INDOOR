#include <assert.h>

#include "tuya_session_guard.h"

int main(void)
{
    assert(tuya_session_standby_touch_allowed(false, false));
    assert(!tuya_session_standby_touch_allowed(true, false));
    assert(tuya_session_standby_touch_allowed(true, true));

    assert(!tuya_session_standby_shortcut_allowed(true, true));
    assert(tuya_session_standby_shortcut_allowed(true, false));
    assert(tuya_session_standby_shortcut_allowed(false, true));

    return 0;
}
