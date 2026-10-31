/* TC
 * id:    hp_neg_expand_null
 * area:  heap
 * funcs: _expand
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _expand
 * note:  PROBING _expand(NULL,n): expect invalid-parameter handler called once, NULL result, errno EINVAL. Docs vague; may not trigger IPH on all CRTs.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>

static volatile size_t v_n = 16;

TC_CASE(expand_null)
{
    void *r;
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0; errno = 0;
    r = _expand(NULL, v_n);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(expand_null); return tc_end(); }
