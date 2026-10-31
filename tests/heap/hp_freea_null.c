/* TC
 * id:    hp_freea_null
 * area:  heap
 * funcs:
 * procs: _freea
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _freea ("if memblock is a null pointer, the function does nothing")
 * note:  _freea is an exported function in msvcr90..120 but an inline header function for ucrtbase, so ucrt is excluded.
 */
#include "tc.h"
#include <errno.h>

typedef void (__cdecl *freea_t)(void *);
static freea_t p_freea;

TC_CASE(null_noop)
{
    if (!p_freea) { CHECK(p_freea != NULL); return; }
    errno = 1234;
    p_freea(NULL);
    CHECK_EQ_I(errno, 1234);
}

int main(void) { tc_begin(); p_freea = (freea_t)tc_crt_proc("_freea"); TC_RUN(null_noop); return tc_end(); }
