#ifndef TUYA_SESSION_GUARD_H
#define TUYA_SESSION_GUARD_H

#include <stdbool.h>

typedef enum
{
    TUYA_SESSION_TARGET_HOME,
    TUYA_SESSION_TARGET_STANDBY,
    TUYA_SESSION_TARGET_TIME_DISPLAY,
    TUYA_SESSION_TARGET_MEDIA,
    TUYA_SESSION_TARGET_WIFI,
    TUYA_SESSION_TARGET_UPGRADE,
    TUYA_SESSION_TARGET_MONITOR,
    TUYA_SESSION_TARGET_CCTV,
    TUYA_SESSION_TARGET_INTERPHONE,
    TUYA_SESSION_TARGET_TRANSFER,
    TUYA_SESSION_TARGET_OTHER,
} tuya_session_target;

bool tuya_session_target_allowed(tuya_session_target target);
bool tuya_session_should_ignore_indoor_busy(bool tuya_session_active);
bool tuya_session_keep_shared_media(bool tuya_client_active,
                                    bool background_tuya_monitor_active);
bool tuya_session_keep_shared_video(bool tuya_client_active);
bool tuya_session_local_monitor_allowed(bool outdoor_busy,
                                        bool local_video_client_active,
                                        bool tuya_talk_active);
bool tuya_session_preserve_outdoor_tuya_order(bool local_video_client_active);
bool tuya_session_standby_touch_allowed(bool outdoor_talking,
                                        bool local_video_client_active);
bool tuya_session_standby_shortcut_allowed(bool tuya_talk_active,
                                           bool requires_local_audio);
bool tuya_session_indoor_call_blocked(bool local_video_client_active,
                                      bool family_monitor_active,
                                      bool local_monitor_entered_from_tuya,
                                      bool tuya_talk_active);

#endif
