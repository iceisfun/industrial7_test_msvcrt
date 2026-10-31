/* TC
 * id:    hp_neg_calloc_huge
 * area:  heap
 * funcs: calloc free
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs calloc (ENOMEM)
 * note:  NULL asserted everywhere; errno=ENOMEM only on ucrtbase.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_a, v_b;

static void try_calloc(size_t a, size_t b, int line)
{
    void *p; errno = 0; v_a = a; v_b = b; p = calloc(v_a, v_b);
    if (p != NULL || (TC_AT_LEAST(140) && errno != ENOMEM)) tc_fail_ints(__FILE__, line, "calloc(a,b)!=NULL || errno", "ENOMEM", p != NULL ? 1 : errno, ENOMEM);
    if (p) free(p);
}
TC_CASE(one_max) { try_calloc(1, SIZE_MAX, __LINE__); }
TC_CASE(max_one) { try_calloc(SIZE_MAX, 1, __LINE__); }
TC_CASE(mega_by_tera) { if (!TC_X64) TC_SKIP("x64 only");
#ifdef _WIN64
    try_calloc((size_t)1 << 20, (size_t)1 << 40, __LINE__);
#endif
}
int main(void) { tc_begin(); TC_RUN(one_max); TC_RUN(max_one); TC_RUN(mega_by_tera); return tc_end(); }
