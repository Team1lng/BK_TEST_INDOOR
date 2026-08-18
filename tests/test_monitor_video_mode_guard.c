#include <assert.h>

#include "monitor_video_mode_guard.h"

int main(void)
{
	assert(monitor_local_video_decode_required(false));
	assert(!monitor_local_video_decode_required(true));
	assert(monitor_background_clear_required(false));
	assert(!monitor_background_clear_required(true));

	return 0;
}
