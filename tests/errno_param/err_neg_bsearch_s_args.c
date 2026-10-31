/* TC
 * id:    err_neg_bsearch_s_args
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: bsearch_s _set_invalid_parameter_handler _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * note:  The NULL-key row is split into err_neg_probe_bsearch_s_null_key (older CRTs may not validate key).
 * ref:   MS docs bsearch_s (parameter validation table: NULL base with number>0, NULL compare, size 0 -> EINVAL, NULL return)
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
TC_DECL_PROC(void *, bsearch_s, (const void *, const void *, size_t, size_t, cmp_t, void *));
TC_DECL_PROC(int *, _errno, (void));

static int cmp_calls;
static int __cdecl cmp(void *c, const void *a, const void *b) { int x, y; (void)c; cmp_calls++; if (!a || !b) return 0; x = *(const int *)a; y = *(const int *)b; return x < y ? -1 : x > y; }
static const int arr[4] = { 1, 3, 5, 7 };
static volatile int key = 5;
static const void * volatile v_null = NULL;
static volatile size_t v_sz, v_num;
static cmp_t volatile v_cmp;

#define PRE() do { *tc_p__errno() = 0; tc_iph_count = 0; cmp_calls = 0; } while (0)
#define BAD() do { CHECK(r == NULL); CHECK_EQ_I(*tc_p__errno(), EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(cmp_calls, 0); } while (0)

TC_CASE(sanity)      { void *r; PRE(); v_sz = sizeof(int); v_num = 4; r = tc_p_bsearch_s((const void *)&key, arr, v_num, v_sz, cmp, NULL); CHECK_EQ_P(r, &arr[2]); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(null_base)   { void *r; PRE(); v_sz = sizeof(int); v_num = 4; r = tc_p_bsearch_s((const void *)&key, v_null, v_num, v_sz, cmp, NULL); BAD(); }
TC_CASE(null_cmp)    { void *r; PRE(); v_sz = sizeof(int); v_num = 4; v_cmp = NULL; r = tc_p_bsearch_s((const void *)&key, arr, v_num, v_sz, v_cmp, NULL); BAD(); }
TC_CASE(size_zero)   { void *r; PRE(); v_sz = 0; v_num = 4; r = tc_p_bsearch_s((const void *)&key, arr, v_num, v_sz, cmp, NULL); BAD(); }
TC_CASE(num_zero_ok) { void *r; PRE(); v_sz = sizeof(int); v_num = 0; r = tc_p_bsearch_s((const void *)&key, v_null, v_num, v_sz, cmp, NULL);
                       CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(*tc_p__errno(), 0); CHECK_EQ_I(cmp_calls, 0); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(bsearch_s); TC_LOAD_PROC(_errno);
    if (!tc_p_bsearch_s || !tc_p__errno || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(sanity); TC_RUN(null_base); TC_RUN(null_cmp); TC_RUN(size_zero); TC_RUN(num_zero_ok);
    return tc_end();
}
