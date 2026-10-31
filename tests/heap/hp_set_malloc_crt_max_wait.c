/* TC
 * id:    hp_set_malloc_crt_max_wait
 * area:  heap
 * funcs: _set_malloc_crt_max_wait
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   (docs: sets the maximum wait time for heap allocation retries; ucrtbase does not export it)
 * note:  PROBING that _set_malloc_crt_max_wait returns the previous value: set(0) yields the old value, then set(old) yields 0.
 */
#include "tc.h"
#include <malloc.h>



TC_CASE(returns_previous)
{
    volatile unsigned long zero = 0; unsigned long old, cur;
    old = _set_malloc_crt_max_wait(zero);
    cur = _set_malloc_crt_max_wait(old);
    CHECK_EQ_I(cur, 0);
    CHECK_EQ_I(_set_malloc_crt_max_wait(old), old);
}

int main(void) { tc_begin(); TC_RUN(returns_previous); return tc_end(); }
