/* TC
 * id:    cv_neg_atoi_null_fatal
 * area:  conv
 * funcs: atoi
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs Parameter validation: default handler terminates the process
 * note:  Probes the exact exit status when atoi(NULL) hits the default invalid-parameter handler. Documented part: process dies abnormally (nonzero, not the 77 sentinel). Likely 0xC0000409 on ucrtbase; msvcr80-120 may show 0xC0000417 or a Watson/abort code; only nonzero/!=77 asserted for older CRTs.
 *        OBSERVED (Win10 22H2 19045 ucrtbase, runs stable): exactly 0xC0000409 once tc_iph_default() restores the real default handler (mingw startup installs a silent one); the guard was also corrected from the never-defined TC_UCRT to TC_AT_LEAST(140).
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile nul;

TC_CASE(sanity_path) { DWORD code = 0; int ran = tc_spawn_self("sanity", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }
TC_CASE(child_dies)
{
    DWORD code = 0; int ran = tc_spawn_self("nullatoi", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(child_exit_status_probe)
{
    DWORD code = 0; int ran = tc_spawn_self("nullatoi", &code, 20000);
    CHECK(ran);
#if TC_AT_LEAST(140)
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
#else
    CHECK(code == TC_STATUS_FASTFAIL || code == TC_STATUS_INVALID_CRT_PARAM || code == 3 || code == 0xC0000005u);
#endif
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { tc_iph_default();
        if (c[0] == 's') return 77;
        (void)atoi(nul);
        return 77;
    }
    TC_RUN(sanity_path); TC_RUN(child_dies); TC_RUN(child_exit_status_probe);
    return tc_end();
}
