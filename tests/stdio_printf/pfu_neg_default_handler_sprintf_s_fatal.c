/* TC
 * id:    pfu_neg_default_handler_sprintf_s_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _snprintf_s swprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter Validation; sprintf_s ("invalid parameter handler is invoked"); default handler terminates the process
 * note:  Child-process tests. Exact status is in pfu_neg_default_handler_sprintf_s_status_probe. tc_begin suppresses abort UI. A control child proves the exe path works (returns 77).
 *        Win10 22H2 fix: fatal children call tc_iph_default() first (mingw crtexe installs a no-op invalid-parameter handler).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(small_buffer)  { DWORD c = 0; CHECK(tc_spawn_self("small", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(null_dest)     { DWORD c = 0; CHECK(tc_spawn_self("nulldst", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(count_ge_size) { DWORD c = 0; CHECK(tc_spawn_self("cnt", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(wide_small)    { DWORD c = 0; CHECK(tc_spawn_self("wide", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(control)       { DWORD c = 0; CHECK(tc_spawn_self("control", &c, 20000)); CHECK_EQ_U(c, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[16]; wchar_t w[4]; if (!(c[0] == 'c' && c[1] == 'o')) tc_iph_default();
        char *volatile nb = NULL; volatile size_t two = 2, one = 1, ten = 10, sixteen = 16;
        if (c[0] == 'c' && c[1] == 'o') { int r = sprintf_s(b, sixteen, "abc"); return r == 3 ? 77 : 78; }
        if (c[0] == 's') sprintf_s(b, two, "abc");
        else if (c[0] == 'n') sprintf_s(nb, ten, "x");
        else if (c[0] == 'c') _snprintf_s(b, two, 5, "abc");
        else swprintf_s(w, one, L"ab");
        return 77;
    }
    TC_RUN(small_buffer); TC_RUN(null_dest); TC_RUN(count_ge_size); TC_RUN(wide_small); TC_RUN(control);
    return tc_end();
}
