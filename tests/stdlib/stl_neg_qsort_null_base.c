/* TC
 * id:    stl_neg_qsort_null_base
 * area:  stdlib
 * kind:  negative
 * funcs: qsort
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs qsort "Parameter validation": base NULL (num != 0), width 0, compare NULL -> EINVAL + invalid parameter handler
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int calls;
static int __cdecl cmp(const void *a, const void *b) { calls++; return (a && b) ? *(const int *)a - *(const int *)b : 0; }

TC_CASE(null_base)
{
    tc_iph_count = 0; errno = 0; calls = 0;
    qsort(NULL, 5, sizeof(int), cmp);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
}
TC_CASE(zero_width)
{
    int a[5] = { 5, 4, 3, 2, 1 };
    tc_iph_count = 0; errno = 0; calls = 0;
    qsort(a, 5, 0, cmp);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
    CHECK_EQ_I(a[0], 5); CHECK_EQ_I(a[1], 4); CHECK_EQ_I(a[2], 3); CHECK_EQ_I(a[3], 2); CHECK_EQ_I(a[4], 1);
}
TC_CASE(null_compare)
{
    int a[5] = { 5, 4, 3, 2, 1 };
    tc_iph_count = 0; errno = 0;
    qsort(a, 5, sizeof a[0], NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(a[0], 5); CHECK_EQ_I(a[4], 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(zero_width); TC_RUN(null_compare); TC_RUN(null_base);
    return tc_end();
}
