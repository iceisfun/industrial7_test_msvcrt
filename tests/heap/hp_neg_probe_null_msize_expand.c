/* TC
 * id:    hp_neg_probe_null_msize_expand
 * area:  heap
 * kind:  negative
 * funcs: _msize _expand
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _msize, _expand (parameter validation)
 * note:  PROBING that _msize(NULL) returns (size_t)-1 and _expand(NULL, n) returns NULL, each setting errno=EINVAL and invoking the
 *        invalid-parameter handler once.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>

TC_CASE(msize_null)
{
    void *volatile p = NULL; size_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _msize(p);
    CHECK_EQ_U(r, (size_t)-1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(expand_null)
{
    void *volatile p = NULL; volatile size_t n = 16; void *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _expand(p, n);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(msize_null); TC_RUN(expand_null); return tc_end(); }
