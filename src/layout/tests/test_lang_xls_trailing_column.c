#include <assert.h>
#include <stddef.h>

#include "lang_xls.h"

int main(void)
{
    assert(lang_xls_init(0) != NULL);
    assert(lang_xls_language_num_get() == 12);
    assert(lang_xls_null_str_num_get() == 0);

    return 0;
}
