#ifndef _MONITOR_VIDEO_MODE_GUARD_H_
#define _MONITOR_VIDEO_MODE_GUARD_H_

#include <stdbool.h>

static inline bool monitor_local_video_decode_required(bool tuya_background_session)
{
	return !tuya_background_session;
}

static inline bool monitor_local_video_output_required(bool tuya_background_session, bool local_monitor_active)
{
	return !tuya_background_session && local_monitor_active;
}

static inline bool monitor_background_clear_required(bool tuya_background_session)
{
	return !tuya_background_session;
}

#endif
