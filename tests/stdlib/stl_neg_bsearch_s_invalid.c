/* TC
 * id:    stl_neg_bsearch_s_invalid
 * area:  stdlib
 * kind:  negative
 * funcs:
 * procs: bsearch_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs bsearch_s "Parameter validation"
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(void *, bsearch_s, (const void *, const void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *));

static int calls;
static int __cdecl cmp(void *ctx, const void *k, const void *e) { (void)ctx; calls++; return (k && e) ? *(const int *)k - *(const int *)e : 0; }
static int arr[3] = { 1, 2, 3 };

#define EXPECT_FAIL(call) do { tc_iph_count = 0; errno = 0; calls = 0; \
    CHECK((call) == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0); } while (0)

TC_CASE(null_base)    { int k = 2, c = 0; if (!tc_p_bsearch_s) TC_SKIP("no bsearch_s"); EXPECT_FAIL(tc_p_bsearch_s(&k, NULL, 3, sizeof(int), cmp, &c)); }
TC_CASE(zero_width)   { int k = 2, c = 0; if (!tc_p_bsearch_s) TC_SKIP("no bsearch_s"); EXPECT_FAIL(tc_p_bsearch_s(&k, arr, 3, 0, cmp, &c)); }
TC_CASE(null_compare) { int k = 2, c = 0; if (!tc_p_bsearch_s) TC_SKIP("no bsearch_s"); EXPECT_FAIL(tc_p_bsearch_s(&k, arr, 3, sizeof(int), NULL, &c)); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(bsearch_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(null_base); TC_RUN(zero_width); TC_RUN(null_compare);
    return tc_end();
}
