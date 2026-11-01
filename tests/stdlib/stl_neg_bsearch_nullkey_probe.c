/* TC
 * id:    stl_neg_bsearch_nullkey_probe
 * area:  stdlib
 * kind:  negative
 * funcs: bsearch
 * procs: bsearch_s
 * crt:   >=140
 * tier:  p1
 * conf:  observed
 * ref:   MS docs bsearch / bsearch_s "Parameter validation"
 * note:  PROBE result: on Win10 22H2 ucrtbase bsearch/bsearch_s do NOT reject a NULL key (num > 0, valid base/width/compare):
 *        errno stays 0, no invalid-parameter call, the comparator is invoked with the NULL key (old expectation: NULL return,
 *        EINVAL, handler call, comparator never invoked, was wrong).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(void *, bsearch_s, (const void *, const void *, size_t, size_t, int (__cdecl *)(void *, const void *, const void *), void *));

static int calls;
static int __cdecl cmp(const void *k, const void *e) { calls++; return (k && e) ? *(const int *)k - *(const int *)e : 0; }
static int __cdecl cmps(void *c, const void *k, const void *e) { (void)c; return cmp(k, e); }
static int arr[3] = { 1, 2, 3 };

/* OBSERVED on Win10 22H2 ucrtbase: a NULL key is NOT validated; the comparator is called with it (our comparator
 * treats a NULL key as equal), so the first probed element is returned; errno stays 0, no handler call. */
TC_CASE(bsearch_null_key)
{
    void *r;
    tc_iph_count = 0; errno = 0; calls = 0;
    r = bsearch(NULL, arr, 3, sizeof(int), cmp);
    CHECK(r != NULL);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); CHECK(calls >= 1);
}
TC_CASE(bsearch_s_null_key)
{
    int c = 0; void *r;
    if (!tc_p_bsearch_s) TC_SKIP("no bsearch_s");
    tc_iph_count = 0; errno = 0; calls = 0;
    r = tc_p_bsearch_s(NULL, arr, 3, sizeof(int), cmps, &c);
    CHECK(r != NULL);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); CHECK(calls >= 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(bsearch_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(bsearch_null_key); TC_RUN(bsearch_s_null_key);
    return tc_end();
}
