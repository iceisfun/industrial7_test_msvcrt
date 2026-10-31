/* TC
 * id:    stl_neg_nullbase_zero_probe
 * area:  stdlib
 * kind:  negative
 * funcs: qsort bsearch
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs qsort / bsearch "Parameter validation" (base NULL is invalid only when num != 0)
 * note:  PROBE: NULL base with num == 0 is accepted silently (no invalid-parameter handler, errno untouched,
 *        bsearch returns NULL, comparator never called). ISO C technically makes a NULL pointer argument UB
 *        even when nmemb is 0, and old CRTs may differ, so this is isolated and limited to ucrtbase.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int calls;
static int __cdecl cmp(const void *a, const void *b) { calls++; return (a && b) ? *(const int *)a - *(const int *)b : 0; }

TC_CASE(qsort_null_zero)
{
    tc_iph_count = 0; errno = 0; calls = 0;
    qsort(NULL, 0, sizeof(int), cmp);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(calls, 0);
}
TC_CASE(bsearch_null_zero)
{
    int k = 2;
    tc_iph_count = 0; errno = 0; calls = 0;
    CHECK(bsearch(&k, NULL, 0, sizeof(int), cmp) == NULL);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(calls, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(qsort_null_zero); TC_RUN(bsearch_null_zero);
    return tc_end();
}
