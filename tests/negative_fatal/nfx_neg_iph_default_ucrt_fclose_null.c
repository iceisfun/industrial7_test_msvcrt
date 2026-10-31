/* TC
 * id:    nfx_neg_iph_default_ucrt_fclose_null
 * area:  negative_fatal
 * kind:  negative
 * funcs: fclose
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fclose (NULL stream invokes invalid parameter handler)
 * note:  Probes exact status 0xC0000409; only termination is documented.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(fclose_null)
{
    DWORD c = 0; int ran = tc_spawn_self("fclose_null", &c, 20000);
    CHECK(ran); CHECK(c != 77);
    CHECK_EQ_U(c, TC_STATUS_FASTFAIL);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) { FILE *volatile f = NULL; fclose(f); return 77; }
    TC_RUN(fclose_null);
    return tc_end();
}
