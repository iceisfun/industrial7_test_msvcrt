/* TC
 * id:    err_neg_probe_bsearch_s_null_key
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: bsearch_s _set_invalid_parameter_handler _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  observed
 * ref:   MS docs bsearch_s (parameter validation table lists key == NULL -> EINVAL)
 * note:  Observed on Win10 22H2 19045 ucrtbase: bsearch_s with key == NULL does not validate the key (docs list NULL key as an
 *        invalid parameter, real ucrtbase does not): the comparator is invoked with a NULL key, no handler call, errno
 *        unchanged. DEVIATES from C11 K.3.6.3.1p2 (key must be non-NULL when nmemb > 0) and from the MS docs table (NULL key -> EINVAL +
 *        handler). The observed expectation is guarded by TC_CRT_VER >= 140; the older CRT branch keeps the original docs
 *        expectation (NULL, EINVAL, handler once, no compare) and is [unverified] (not run on the reference).
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

TC_CASE(null_key)
{
    void *r; PRE(); v_sz = sizeof(int); v_num = 4; r = tc_p_bsearch_s(v_null, arr, v_num, v_sz, cmp, NULL);
    /* observed: key == NULL is NOT validated; the comparator is called with a NULL key (our cmp answers 0 = "equal"), so the
     * element it was compared against comes back, errno is untouched and the handler never runs */
#if TC_CRT_VER >= 140
    CHECK(r != NULL); CHECK((const int *)r >= arr && (const int *)r < arr + 4);
    CHECK_EQ_I(*tc_p__errno(), 0); CHECK_EQ_I(tc_iph_count, 0); CHECK(cmp_calls >= 1);
#else   /* [unverified] original docs expectation */
    CHECK(r == NULL); CHECK_EQ_I(*tc_p__errno(), EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(cmp_calls, 0);
#endif
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(bsearch_s); TC_LOAD_PROC(_errno);
    if (!tc_p_bsearch_s || !tc_p__errno || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(null_key);
    return tc_end();
}
