/* TC
 * id:    stl_neg_qsort_zero_count_bad_args_probe
 * area:  stdlib
 * kind:  negative
 * funcs: qsort bsearch
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs qsort/bsearch "Parameter validation"
 * note:  PROBES whether the UCRT validates width/compare BEFORE short-circuiting on num == 0. Expectation (UCRT validates first):
 *        iph fires once with errno EINVAL for NULL compare or zero width even though num == 0. Real Windows is the ground truth.
 *        Only "no crash and bsearch returns NULL" is certain.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef int (__cdecl *cmp_t)(const void *, const void *);
static int __cdecl cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }
static int base[2] = { 1, 2 };

TC_CASE(qsort_null_cmp)  { cmp_t volatile c = NULL; volatile size_t n = 0; tc_iph_count = 0; errno = 0; qsort(base, n, 4, c); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(qsort_zero_width){ volatile size_t w = 0, n = 0; tc_iph_count = 0; errno = 0; qsort(base, n, w, cmp); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(bsearch_zero_width)
{
    int k = 1; volatile size_t w = 0, n = 0; void *r;
    tc_iph_count = 0; errno = 0; r = bsearch(&k, base, n, w, cmp);
    CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(bsearch_null_cmp)
{
    int k = 1; cmp_t volatile c = NULL; volatile size_t n = 0; void *r;
    tc_iph_count = 0; errno = 0; r = bsearch(&k, base, n, 4, c);
    CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(qsort_null_cmp); TC_RUN(qsort_zero_width); TC_RUN(bsearch_zero_width); TC_RUN(bsearch_null_cmp);
    return tc_end();
}
