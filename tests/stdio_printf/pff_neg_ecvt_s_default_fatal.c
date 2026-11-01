/* TC
 * id:    pff_neg_ecvt_s_default_fatal
 * area:  stdio_printf
 * funcs: _ecvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: with no handler an invalid parameter terminates the process
 * note:  msvcrt.dll and msvcr80 excluded (msvcr80 import lib lacks the _s funcs): no invalid-parameter machinery, it returns EINVAL without terminating.
 *        Child process calls _ecvt_s(NULL,...) with the default handler; exit code must be neither 0 nor the sentinel 77.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullbuf", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
#if TC_CRT_VER >= 140
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL); /* observed Win10 22H2 19045 ucrtbase */
#endif
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        int d, s;
        tc_iph_default(); /* mingw crt startup installs a no-op handler; restore the CRT default (fatal) */
        _ecvt_s(NULL, 10, 1.5, 3, &d, &s);
        return 77;
    }
    TC_RUN(child_terminates);
    return tc_end();
}
