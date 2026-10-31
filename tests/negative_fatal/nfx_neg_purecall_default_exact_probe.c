/* TC
 * id:    nfx_neg_purecall_default_exact_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: _purecall
 * procs: 
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs R6025
 * note:  PROBES exact exit status of default pure virtual termination: abort path (3) on msvcr80..120, and on ucrtbase (believed _invoke_watson -> fast fail 0xC0000409). Only a guess.
 */
#include "tc.h"

extern void __cdecl _purecall(void);
TC_CASE(exact_status)
{
    DWORD c = 0; CHECK(tc_spawn_self("pc", &c, 20000));
#if TC_CRT_VER >= 140
    CHECK_EQ_U(c, TC_STATUS_FASTFAIL);
#else
    CHECK_EQ_U(c, 3);
#endif
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _purecall(); return 77; }
    TC_RUN(exact_status); return tc_end();
}
