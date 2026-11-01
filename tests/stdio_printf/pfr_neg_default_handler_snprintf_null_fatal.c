/* TC
 * id:    pfr_neg_default_handler_snprintf_null_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _snwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter validation / _set_invalid_parameter_handler (default handler terminates the process); _snprintf (NULL buffer with count > 0, NULL format are invalid parameters)
 * note:  Child-process test. Each fatal child makes ONE bad call; the exit code must not be 77 (reached only if the call returned)
 *        and not 0. snprintf (C99) is only available as an inline/export on ucrtbase and is tested there only. The control
 *        child installs a handler and expects -1 plus one handler call per bad call, exiting 77.
 *        Win10 22H2 fix: fatal children call tc_iph_default() first (mingw crtexe installs a no-op handler).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
#define HAVE_C99 1
#endif

TC_CASE(snprintf_legacy_null_fatal)  { DWORD c = 0; CHECK(tc_spawn_self("sn_null", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(snwprintf_null_fatal)        { DWORD c = 0; CHECK(tc_spawn_self("snw_null", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
#ifdef HAVE_C99
TC_CASE(snprintf_c99_null_fatal)     { DWORD c = 0; CHECK(tc_spawn_self("c99_null", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
#endif
TC_CASE(snprintf_legacy_nullfmt_fatal) { DWORD c = 0; CHECK(tc_spawn_self("sn_nullfmt", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(control)                     { DWORD c = 0; CHECK(tc_spawn_self("control", &c, 20000)); CHECK_EQ_U(c, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char *volatile nb = NULL; wchar_t *volatile nw = NULL; const char *volatile nf = NULL; char buf[8];
        if (c[0] == 'c' && c[1] == 'o') {
            int r1, r2, r3, ok;
            if (!tc_iph_install()) return 3;
            tc_iph_count = 0;
            r1 = _snprintf(nb, 5, "x");
            r2 = _snwprintf(nw, 5, L"x");
            r3 = _snprintf(buf, 5, nf);
            ok = (r1 == -1 && r2 == -1 && r3 == -1 && tc_iph_count == 3);
#ifdef HAVE_C99
            ok = ok && snprintf(nb, 5, "x") == -1 && tc_iph_count == 4;
#endif
            return ok ? 77 : 100 + tc_iph_count;
        }
        tc_iph_default();
        if (!lstrcmpA(c, "sn_null")) _snprintf(nb, 5, "x");
        else if (!lstrcmpA(c, "snw_null")) _snwprintf(nw, 5, L"x");
        else if (!lstrcmpA(c, "sn_nullfmt")) _snprintf(buf, 5, nf);
#ifdef HAVE_C99
        else if (!lstrcmpA(c, "c99_null")) snprintf(nb, 5, "x");
#endif
        return 77;
    }
    TC_RUN(snprintf_legacy_null_fatal); TC_RUN(snwprintf_null_fatal);
#ifdef HAVE_C99
    TC_RUN(snprintf_c99_null_fatal);
#endif
    TC_RUN(snprintf_legacy_nullfmt_fatal); TC_RUN(control);
    return tc_end();
}
