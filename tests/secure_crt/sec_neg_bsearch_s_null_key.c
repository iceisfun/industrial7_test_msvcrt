/* TC
 * id:    sec_neg_bsearch_s_null_key
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: bsearch_s _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs bsearch_s: parameter validation table (NULL base with num>0, size==0, NULL compare, NULL key -> EINVAL + invalid-parameter handler)
 * note:  Observed on Win10 22H2 19045 ucrtbase: bsearch_s(key=NULL, valid array) is not rejected with EINVAL as the docs table
 *        implies; the comparator is called with the NULL key and the dereferencing comparator crashes the process with
 *        0xC0000005. Class C: moved to a child process asserting the access violation (see err_neg_probe_bsearch_s_null_key for
 *        the non-crashing view). DEVIATES from C11 K.3.6.3.1p2 and from the MS docs table (NULL key -> EINVAL + handler).
 *        The crash expectation is guarded by TC_CRT_VER >= 140; the msvcr90..120 branch keeps the original docs expectation
 *        (NULL, EINVAL 22, handler called) in-process and is [unverified] (not run on the reference).
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *cmp_t)(void *, const void *, const void *);
typedef void *(__cdecl *bsearch_s_t)(const void *, const void *, size_t, size_t, cmp_t, void *);
typedef int *(__cdecl *errno_fn)(void);
static bsearch_s_t p_b; static errno_fn p_errno;
static int calls;
static int __cdecl cmp(void *c, const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; (void)c; calls++; return x < y ? -1 : x > y; }

TC_CASE(bsearch_null_key_reaches_comparator)
{
    /* observed: bsearch_s does NOT validate key == NULL; it calls the comparator with the NULL key, so a comparator that dereferences
     * both arguments (as this one does) takes an access violation. The fatal call therefore runs in a child. */
#if TC_CRT_VER >= 140
    DWORD code = 0; int ran = tc_spawn_self("nullkey", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
#else   /* [unverified] original docs expectation */
    static const int a[3] = { 1, 2, 3 }; void *r;
    tc_iph_install();
    *p_errno() = 0; tc_iph_count = 0;
    r = p_b(NULL, a, 3, sizeof a[0], cmp, NULL);
    CHECK(r == NULL); CHECK_EQ_I(*p_errno(), 22); CHECK(tc_iph_count >= 1);
#endif
}
int main(void)
{
    tc_begin();
    p_b = (bsearch_s_t)tc_crt_proc("bsearch_s"); p_errno = (errno_fn)tc_crt_proc("_errno");
    CHECK(p_b && p_errno); if (!(p_b && p_errno)) return tc_end();
    if (tc_child_name() != NULL) {
        static const int a[3] = { 1, 2, 3 }; void *r;
        tc_iph_install();
        r = p_b(NULL, a, 3, sizeof a[0], cmp, NULL);
        (void)r;
        return 77;
    }
    TC_RUN(bsearch_null_key_reaches_comparator);
    return tc_end();
}
