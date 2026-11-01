/* TC
 * id:    pff_neg_invalid_spec_default_fatal
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf; Parameter validation
 * note:  child process, default handler restored with tc_iph_default() (mingw startup installs a no-op one).
 *        OBSERVED Win10 22H2 19045 ucrtbase: non-secure sprintf does NOT validate the conversion char: "%f %y" prints "y" literally and the child survives (exit 77),
 *        so the old "fatal" expectation was wrong for UCRT (kept for msvcr*). The secure sprintf_s("%f %y") IS fatal (0xC0000409): case fatal_child_s.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(fatal_child)
{
    DWORD code = 0; int ran = tc_spawn_self("badspec", &code, 20000);
    CHECK(ran);
#if TC_CRT_VER >= 140
    CHECK_EQ_U(code, 77);
#else
    CHECK(code != 0); CHECK(code != 77);
#endif
}
TC_CASE(fatal_child_s)
{
    DWORD code = 0; int ran = tc_spawn_self("badspec_s", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
#if TC_CRT_VER >= 140
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
#endif
}
int main(void)
{
    tc_begin();
    const char *c = tc_child_name();
    if (c) { char b[64]; volatile double v = 1.5; const char *volatile f = "%f %y"; tc_iph_default();
        if (c[7] == '_') sprintf_s(b, sizeof b, f, v, 1); else sprintf(b, f, v, 1); return 77; }
    TC_RUN(fatal_child); TC_RUN(fatal_child_s);
    return tc_end();
}
