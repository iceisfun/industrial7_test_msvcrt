/* TC
 * id:    gwc_neg_wcstol_l_fatal_status_probe
 * area:  conv
 * funcs: _wcstol_l _create_locale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   Parameter validation; ucrt default invalid parameter handler calls __fastfail
 * note:  PROBES: the default handler terminates the child with exactly STATUS_STACK_BUFFER_OVERRUN (0xC0000409).
 *        OBSERVED (Win10 22H2 19045 ucrtbase, runs stable): exit 0xC0000409 after tc_iph_default() restores the real default handler (mingw startup installs a silent one).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

static const wchar_t *volatile vp;
static volatile int vbase = 1;

TC_CASE(status)
{
    DWORD code = 0; int ran = tc_spawn_self("badbase", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { tc_iph_default();
        _locale_t loc = _create_locale(LC_ALL, "C");
        vp = L"1"; (void)_wcstol_l(vp, NULL, vbase, loc);
        return 77;
    }
    TC_RUN(status);
    return tc_end();
}
