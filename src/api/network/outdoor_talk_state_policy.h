#ifndef OUTDOOR_TALK_STATE_POLICY_H
#define OUTDOOR_TALK_STATE_POLICY_H

#include <stdbool.h>

static inline bool outdoor_talk_busy_effective(bool raw_busy,
                                               unsigned long long now_ms,
                                               unsigned long long hang_ms,
                                               unsigned long long grace_ms)
{
    if (!raw_busy)
    {
        return false;
    }
    if (hang_ms != 0 && now_ms - hang_ms < grace_ms)
    {
        return false;
    }
    return true;
}

#endif
