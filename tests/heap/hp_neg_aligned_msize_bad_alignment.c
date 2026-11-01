/* TC
 * id:    hp_neg_aligned_msize_bad_alignment
 * area:  heap
 * kind:  negative
 * funcs: _aligned_msize _aligned_malloc _aligned_free
 * crt:   >=90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _aligned_msize (alignment must be a power of 2; invalid value -> invalid-parameter handler, EINVAL, returns (size_t)-1)
 * note:  The docs list the power-of-two requirement for alignment as a parameter validation rule.
 *        Win10 22H2 19045 (observed): _aligned_msize does NOT validate that alignment is a power of two (alignments 3,12,24,100: no handler call,
 *        errno untouched, return value is a size computed from the header (not -1)). Observed values for a 64-byte block of alignment 16: 72, 68, 56, -20 (size_t) for
 *        alignments 3, 12, 24, 100, i.e. valid_result + 16 - max(alignment, sizeof(void*)); that formula is asserted (ucrtbase 19045 only,
 *        TC_AT_LEAST(140); [unverified] on msvcr90..120 where the original -1/EINVAL/handler expectation is kept).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_a;

TC_CASE(non_power_of_two)
{
    static const size_t bad[] = { 3, 12, 24, 100 };
    void *p; size_t i, r, base;
    CHECK(tc_iph_install());
    p = _aligned_malloc(64, 16);
    CHECK(p != NULL);
    if (!p) return;
    base = _aligned_msize(p, 16, 0);
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        v_a = bad[i]; errno = 0; tc_iph_count = 0;
        r = _aligned_msize(p, v_a, 0);
#if TC_AT_LEAST(140)
        CHECK_EQ_U(r, base + 16 - (bad[i] < sizeof(void *) ? sizeof(void *) : bad[i]));   /* observed formula, no -1 */
        CHECK_EQ_I(errno, 0);              /* observed: errno untouched */
        CHECK_EQ_I(tc_iph_count, 0);       /* observed: handler not called */
#else
        CHECK_EQ_U(r, (size_t)-1);
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, 1);
#endif
    }
    _aligned_free(p);
}
TC_CASE(valid_still_works)
{
    void *p = _aligned_malloc(64, 16); size_t r;
    CHECK(p != NULL);
    if (!p) return;
    r = _aligned_msize(p, 16, 0);
    CHECK(r != (size_t)-1);
    CHECK(r >= 64);
    _aligned_free(p);
}
int main(void) { tc_begin(); TC_RUN(non_power_of_two); TC_RUN(valid_still_works); return tc_end(); }
