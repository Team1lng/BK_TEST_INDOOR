#include <assert.h>

#include "tuya_stream_keepalive_policy.h"

int main(void)
{
	assert(!tuya_stream_keepalive_due(false, 5000, 0, 2000));
	assert(tuya_stream_keepalive_due(true, 0, 0, 2000));
	assert(tuya_stream_keepalive_due(true, 1999, 0, 2000));
	assert(tuya_stream_keepalive_due(true, 2000, 0, 2000));
	assert(!tuya_stream_keepalive_due(true, 3999, 2000, 2000));
	assert(tuya_stream_keepalive_due(true, 4000, 2000, 2000));

	return 0;
}
