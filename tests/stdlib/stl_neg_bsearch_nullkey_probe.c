/* TC
 * id:    stl_neg_bsearch_nullkey_probe
 * area:  stdlib
 * kind:  negative
 * funcs: bsearch
 * procs: bsearch_s
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs bsearch / bsearch_s "Parameter validation"
 * note:  PROBE: a NULL key (num > 0, valid base/width/compare) is rejected with NULL return, errno EINVAL, one
 *        invalid-parameter handler call, and the comparator is never invoked. Believed from the docs table but
 *        not certain the check exists in every CRT, so isolated here and limited to ucrtbase.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(void *, bsearch_s, (const void *, const void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *));

static int calls;
static int __cdecl cmp(const void *k, const void *e) { calls++; return (k && e) ? *(const int *)k - *(const int *)e : 0; }
static int __cdecl cmps(void *c, const void *k, const void *e) { (void)c; return cmp(k, e); }
static int arr[3] = { 1, 2, 3 };

TC_CASE(bsearch_null_key)
{
    tc_iph_count = 0; errno = 0; calls = 0;
    CHECK(bsearch(NULL, arr, 3, sizeof(int), cmp) == NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
}
TC_CASE(bsearch_s_null_key)
{
    int c = 0;
    if (!tc_p_bsearch_s) TC_SKIP("no bsearch_s");
    tc_iph_count = 0; errno = 0; calls = 0;
    CHECK(tc_p_bsearch_s(NULL, arr, 3, sizeof(int), cmps, &c) == NULL);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(calls, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(bsearch_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(bsearch_null_key); TC_RUN(bsearch_s_null_key);
    return tc_end();
}
