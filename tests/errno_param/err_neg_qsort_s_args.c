/* TC
 * id:    err_neg_qsort_s_args
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: qsort_s _set_invalid_parameter_handler _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs qsort_s (parameter validation table: NULL base with number>0, size 0, NULL compare -> EINVAL, no sort)
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
TC_DECL_PROC(void, qsort_s, (void *, size_t, size_t, cmp_t, void *));
TC_DECL_PROC(int *, _errno, (void));

static int cmp_calls;
static int __cdecl cmp(void *c, const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; (void)c; cmp_calls++; return x < y ? -1 : x > y; }
static const int orig[4] = { 4, 2, 3, 1 };
static int arr[4];
static void * volatile v_null = NULL;
static volatile size_t v_sz, v_num;
static cmp_t volatile v_cmp;

static void pre(void) { int i; for (i = 0; i < 4; i++) arr[i] = orig[i]; *tc_p__errno() = 0; tc_iph_count = 0; cmp_calls = 0; v_sz = sizeof(int); v_num = 4; v_cmp = cmp; }
#define UNTOUCHED() CHECK_MEM(arr, orig, sizeof orig)
#define BAD() do { CHECK_EQ_I(*tc_p__errno(), EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(cmp_calls, 0); UNTOUCHED(); } while (0)

TC_CASE(sanity)      { pre(); tc_p_qsort_s(arr, v_num, v_sz, v_cmp, NULL); CHECK_EQ_I(arr[0], 1); CHECK_EQ_I(arr[3], 4); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(null_base)   { pre(); tc_p_qsort_s(v_null, v_num, v_sz, v_cmp, NULL); BAD(); }
TC_CASE(null_cmp)    { pre(); v_cmp = NULL; tc_p_qsort_s(arr, v_num, v_sz, v_cmp, NULL); BAD(); }
TC_CASE(size_zero)   { pre(); v_sz = 0; tc_p_qsort_s(arr, v_num, v_sz, v_cmp, NULL); BAD(); }
TC_CASE(num_zero_ok) { pre(); v_num = 0; tc_p_qsort_s(v_null, v_num, v_sz, v_cmp, NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(*tc_p__errno(), 0); CHECK_EQ_I(cmp_calls, 0); UNTOUCHED(); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(qsort_s); TC_LOAD_PROC(_errno);
    if (!tc_p_qsort_s || !tc_p__errno || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(sanity); TC_RUN(null_base); TC_RUN(null_cmp); TC_RUN(size_zero); TC_RUN(num_zero_ok);
    return tc_end();
}
