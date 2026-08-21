#include <assert.h>

#include "../../api/network/outdoor_talk_state_policy.h"

int main(void)
{
    assert(!outdoor_talk_busy_effective(false, 5000, 4000, 3000));
    assert(outdoor_talk_busy_effective(true, 5000, 0, 3000));
    assert(!outdoor_talk_busy_effective(true, 5000, 4000, 3000));
    assert(outdoor_talk_busy_effective(true, 7000, 4000, 3000));
    assert(outdoor_talk_busy_effective(true, 5000, 0, 3000));
    return 0;
}
