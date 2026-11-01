/* TC
 * id:    nfx_neg_purecall_default_exact_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: _purecall
 * procs: 
 * crt:   >=80
 * tier:  p2
 * conf:  observed
 * ref:   MS docs R6025
 * note:  Observed on Win10 22H2 19045 ucrtbase: the default _purecall ends in abort(); tc_begin() clears _CALL_REPORTFAULT
 *        (child exit code 3, as on the msvcr80..120 branch), so the ucrtbase child restores it with tc_abort_default() and the
 *        exit status is the fast-fail 0xC0000409. For msvcr80..120 the child does NOT restore the flag (flag cleared -> exit 3, the old
 *        expectation); that branch is [unverified] (not run on the reference).
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
    if (tc_child_name()) {
#if TC_CRT_VER >= 140
        tc_abort_default();
#endif
        _purecall(); return 77; }
    TC_RUN(exact_status); return tc_end();
}
