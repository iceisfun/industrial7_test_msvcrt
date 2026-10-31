/* TC
 * id:    stl_neg_bsearch_invalid
 * area:  stdlib
 * kind:  negative
 * funcs: bsearch
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs bsearch "Parameter validation": returns NULL, errno EINVAL, invalid parameter handler
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int calls;
static int __cdecl cmp(const void *k, const void *e) { calls++; return (k && e) ? *(const int *)k - *(const int *)e : 0; }
static int arr[3] = { 1, 2, 3 };

TC_CASE(null_base)
{
    int k = 2;
    tc_iph_count = 0; errno = 0; calls = 0;
    CHECK(bsearch(&k, NULL, 3, sizeof(int), cmp) == NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
}
TC_CASE(zero_width)
{
    int k = 2;
    tc_iph_count = 0; errno = 0; calls = 0;
    CHECK(bsearch(&k, arr, 3, 0, cmp) == NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
}
TC_CASE(null_compare)
{
    int k = 2;
    tc_iph_count = 0; errno = 0;
    CHECK(bsearch(&k, arr, 3, sizeof(int), NULL) == NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(null_base); TC_RUN(zero_width); TC_RUN(null_compare);
    return tc_end();
}
