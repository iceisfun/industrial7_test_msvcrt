/* TC
 * id:    exp_neg_bsearch_s_null_key
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: bsearch_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs bsearch_s: NULL key/compare -> invalid parameter handler, EINVAL, returns NULL
 * note:  
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
TC_DECL_PROC(void *, bsearch_s, (const void *, const void *, size_t, size_t, cmp_t, void *));
static int __cdecl cmp(void *c, const void *a, const void *b) { (void)c; return *(const int *)a - *(const int *)b; }
static int a[3] = { 1, 3, 5 };
static int k = 3, missing = 4;

TC_CASE(null_key_returns_null_einval) { tc_iph_count = 0; errno = 0; CHECK(tc_p_bsearch_s(NULL, a, 3, sizeof(int), cmp, NULL) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_compare_einval) { tc_iph_count = 0; errno = 0; CHECK(tc_p_bsearch_s(&k, a, 3, sizeof(int), NULL, NULL) == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(not_found_returns_null_no_iph) { tc_iph_count = 0; CHECK(tc_p_bsearch_s(&missing, a, 3, sizeof(int), cmp, NULL) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK(tc_p_bsearch_s(&k, a, 3, sizeof(int), cmp, NULL) == &a[1]); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(bsearch_s); tc_iph_install();
    if (!tc_p_bsearch_s) { tc_write("TC FAIL " TC_TEST_ID "::exported\n"); return 1; }
    TC_RUN(null_key_returns_null_einval); TC_RUN(null_compare_einval); TC_RUN(not_found_returns_null_no_iph);
    return tc_end();
}
