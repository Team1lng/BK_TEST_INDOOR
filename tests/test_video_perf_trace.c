#include <assert.h>

#include "video_perf_trace.h"

int main(void)
{
	video_perf_stats stats;

	video_perf_stats_reset(&stats);
	video_perf_stats_record(&stats, 100, 5, 0, false, 0, 10);
	video_perf_stats_record(&stats, 200, 12, -1, true, 3, 10);

	assert(stats.calls == 2);
	assert(stats.successes == 1);
	assert(stats.failures == 1);
	assert(stats.zero_progress == 1);
	assert(stats.busy_loops == 3);
	assert(stats.slow_calls == 1);
	assert(stats.bytes == 300);
	assert(stats.total_ms == 17);
	assert(stats.max_ms == 12);

	video_perf_stats_reset(&stats);
	assert(stats.calls == 0);
	assert(stats.bytes == 0);

	return 0;
}
