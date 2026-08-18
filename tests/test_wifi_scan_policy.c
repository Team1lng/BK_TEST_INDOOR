#include <assert.h>

#include "wifi_scan_policy.h"

int main(void)
{
	volatile bool running = false;

	assert(wifi_scan_try_begin(&running));
	assert(running);
	assert(!wifi_scan_try_begin(&running));
	wifi_scan_finish(&running);
	assert(!running);
	assert(wifi_scan_try_begin(&running));

	return 0;
}
