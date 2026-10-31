/* TC
 * id:    hp_neg_b2_calloc_wrap
 * area:  heap
 * funcs: calloc free
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs calloc (returns NULL, errno ENOMEM on failure); C11 7.22.3.2
 * note:  msvcrt.dll (60) excluded: overflow checking in its calloc is not documented. calloc(n,s) with n*s overflowing size_t must fail rather than wrap to a small block.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile size_t v_n, v_s;

TC_CASE(wraps_to_small)
{
    void *p; v_n = ((size_t)1 << (sizeof(size_t) * 8 - 1)) + 1; v_s = 2; errno = 0;
    p = calloc(v_n, v_s);
    CHECK(p == NULL);
    if (p) free(p);
}
TC_CASE(wraps_to_small_errno)
{
    void *p; v_n = ((size_t)1 << (sizeof(size_t) * 8 - 1)) + 1; v_s = 2; errno = 0;
    p = calloc(v_n, v_s);
    if (p) free(p);
    else CHECK_EQ_I(errno, ENOMEM);
}
int main(void) { tc_begin(); TC_RUN(wraps_to_small); TC_RUN(wraps_to_small_errno); return tc_end(); }
