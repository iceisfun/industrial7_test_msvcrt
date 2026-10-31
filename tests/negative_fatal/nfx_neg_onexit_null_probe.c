/* TC
 * id:    nfx_neg_onexit_null_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: 
 * procs: _onexit
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _onexit, Parameter validation
 * note:  Probes: _onexit(NULL) is rejected as an invalid parameter (returns NULL, handler count 1). Unsure whether real CRTs validate; msvcrt.dll excluded (no invalid-parameter handler API).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));


TC_CASE(null_rejected)
{
    _onexit_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    volatile _onexit_t f = NULL;
    r = tc_p__onexit(f);
    CHECK(r == NULL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_onexit);
    TC_RUN(null_rejected);
    return tc_end();
}
