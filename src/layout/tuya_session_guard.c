#include "tuya_session_guard.h"

bool tuya_session_target_allowed(tuya_session_target target)
{
    return target == TUYA_SESSION_TARGET_HOME ||
           target == TUYA_SESSION_TARGET_STANDBY ||
           target == TUYA_SESSION_TARGET_TIME_DISPLAY ||
           target == TUYA_SESSION_TARGET_MEDIA ||      
           target == TUYA_SESSION_TARGET_WIFI ||       
           target == TUYA_SESSION_TARGET_TRANSFER ||   
           target == TUYA_SESSION_TARGET_OTHER;         
}


bool tuya_session_should_ignore_indoor_busy(bool tuya_session_active)
{
	return tuya_session_active;
}

bool tuya_session_keep_shared_media(bool tuya_client_active,
                                    bool background_tuya_monitor_active)
{
    (void)background_tuya_monitor_active;

    return tuya_client_active;
}

bool tuya_session_keep_shared_video(bool tuya_client_active)
{
    return tuya_client_active;
}

bool tuya_session_local_monitor_allowed(bool outdoor_busy,
                                        bool local_video_client_active,
                                        bool tuya_talk_active)
{
    (void)outdoor_busy;
    (void)local_video_client_active;

    return !tuya_talk_active;
}

bool tuya_session_local_talk_allowed(bool local_video_client_active,
                                     bool background_tuya_monitor_active,
                                     bool outdoor_busy)
{
    return !local_video_client_active &&
           !background_tuya_monitor_active &&
           !outdoor_busy;
}

bool tuya_session_local_talk_button_allowed(bool local_talk_active,
                                            bool local_video_client_active,
                                            bool background_tuya_monitor_active,
                                            bool outdoor_busy)
{
    return local_talk_active ||
           tuya_session_local_talk_allowed(local_video_client_active,
                                           background_tuya_monitor_active,
                                           outdoor_busy);
}

bool tuya_session_remote_busy_should_interrupt_ui(void)
{
    return false;
}

bool tuya_session_remote_channel_switch_allowed(bool local_monitor_active)
{
    return !local_monitor_active;
}

bool tuya_session_preserve_outdoor_tuya_order(bool local_video_client_active)
{
	return local_video_client_active;
}

bool tuya_session_standby_touch_allowed(bool outdoor_talking,
                                        bool local_video_client_active)
{
	return !outdoor_talking || local_video_client_active;
}

bool tuya_session_standby_shortcut_allowed(bool tuya_talk_active,
                                           bool requires_local_audio)
{
	return !tuya_talk_active || !requires_local_audio;
}

bool tuya_session_indoor_call_blocked(bool local_video_client_active,
                                      bool family_monitor_active,
                                      bool local_monitor_entered_from_tuya,
                                      bool tuya_talk_active)
{
    (void)family_monitor_active;

    return local_video_client_active || local_monitor_entered_from_tuya || tuya_talk_active;
}
