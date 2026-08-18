#include <assert.h>

#include "tuya_online_status_policy.h"

int main(void)
{
	assert(!tuya_online_status_from_callback(0));
	assert(tuya_online_status_from_callback(1));
	assert(tuya_online_status_from_callback(2));

	return 0;
}
