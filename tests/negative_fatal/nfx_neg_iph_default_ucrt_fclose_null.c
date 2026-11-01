/* TC
 * id:    nfx_neg_iph_default_ucrt_fclose_null
 * area:  negative_fatal
 * kind:  negative
 * funcs: fclose
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fclose (NULL stream invokes invalid parameter handler)
 * note:  Observed on Win10 22H2 19045 ucrtbase: with the CRT's default invalid-parameter handler restored in the child
 *        (tc_iph_default(); the mingw startup installs a no-op handler that made the child just return 77) fclose(NULL) ends
 *        the process with STATUS_STACK_BUFFER_OVERRUN 0xC0000409 (fast-fail). Stable over repeated runs. Fatal path runs in a
 *        child process.
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
    if (tc_child_name()) { FILE *volatile f = NULL; tc_iph_default(); fclose(f); return 77; }
    TC_RUN(fclose_null);
    return tc_end();
}
