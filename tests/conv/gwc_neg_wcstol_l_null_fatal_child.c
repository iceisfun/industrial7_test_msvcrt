/* TC
 * id:    gwc_neg_wcstol_l_null_fatal_child
 * area:  conv
 * funcs: _wcstol_l _wcstod_l _create_locale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation (default invalid parameter handler terminates the process); _wcstol_l, _wcstod_l
 * note:  Children run with the default handler; they must die (exit code != 0 and != the sentinel 77). The exact status is probed in gwc_neg_wcstol_l_fatal_status_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

static const wchar_t *volatile vp;
static volatile int vbase;

static void expect_dies(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(sanity) { DWORD code = 0; int ran = tc_spawn_self("sanity", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }
TC_CASE(null_string)  { expect_dies("wcstol_null"); }
TC_CASE(bad_base)     { expect_dies("wcstol_base"); }
TC_CASE(wcstod_null)  { expect_dies("wcstod_null"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { tc_iph_default();
        _locale_t loc = _create_locale(LC_ALL, "C");
        if (c[0] == 's') return 77;
        vp = NULL;
        if (c[5] == 'd') (void)_wcstod_l(vp, NULL, loc);
        else if (c[7] == 'n') (void)_wcstol_l(vp, NULL, 10, loc);
        else { vp = L"1"; vbase = 99; (void)_wcstol_l(vp, NULL, vbase, loc); }
        return 77;
    }
    TC_RUN(sanity); TC_RUN(null_string); TC_RUN(bad_base); TC_RUN(wcstod_null);
    return tc_end();
}
