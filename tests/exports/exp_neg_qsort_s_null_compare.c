/* TC
 * id:    exp_neg_qsort_s_null_compare
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: qsort_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs qsort_s: invalid pointers or size -> invalid parameter handler, EINVAL, no sort
 * note:  num==0 with NULL base is valid (nothing to do).
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
TC_DECL_PROC(void, qsort_s, (void *, size_t, size_t, cmp_t, void *));
static int __cdecl cmp(void *c, const void *a, const void *b) { (void)c; return *(const int *)a - *(const int *)b; }
static int a[3];
static void reset(void) { a[0] = 3; a[1] = 1; a[2] = 2; }
static int unsorted(void) { return a[0] == 3 && a[1] == 1 && a[2] == 2; }

TC_CASE(null_compare_einval) { reset(); tc_iph_count = 0; errno = 0; tc_p_qsort_s(a, 3, sizeof(int), NULL, NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK(unsorted()); }
TC_CASE(null_base_nonzero_num_einval) { tc_iph_count = 0; errno = 0; tc_p_qsort_s(NULL, 3, sizeof(int), cmp, NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(zero_size_einval) { reset(); tc_iph_count = 0; errno = 0; tc_p_qsort_s(a, 3, 0, cmp, NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK(unsorted()); }
TC_CASE(num0_null_base_ok) { tc_iph_count = 0; errno = 0; tc_p_qsort_s(NULL, 0, sizeof(int), cmp, NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(qsort_s); tc_iph_install();
    if (!tc_p_qsort_s) { tc_write("TC FAIL " TC_TEST_ID "::exported\n"); return 1; }
    TC_RUN(null_compare_einval); TC_RUN(null_base_nonzero_num_einval); TC_RUN(zero_size_einval); TC_RUN(num0_null_base_ok);
    return tc_end();
}
