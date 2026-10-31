/* TC
 * id:    pfr_neg_default_handler_scprintf_bad_format_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter validation, _scprintf (NULL format is an invalid parameter: fatal under the default handler); C11 7.21.6.5 snprintf(NULL, 0, ...)
 * note:  Child-process test. _scprintf(NULL) must terminate the process under the default handler, while the pure length query
 *        snprintf(NULL, 0, "abc") must not (exit 77 reached, and it returned 3). Unknown-conversion fatality is probed
 *        in pfr_neg_probe_default_handler_scprintf_unknown_conv_fatal.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(null_format_fatal)     { DWORD c = 0; CHECK(tc_spawn_self("nullfmt", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(length_query_survives) { DWORD c = 0; CHECK(tc_spawn_self("query", &c, 20000)); CHECK_EQ_U(c, 77); }
TC_CASE(control)               { DWORD c = 0; CHECK(tc_spawn_self("control", &c, 20000)); CHECK_EQ_U(c, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        const char *volatile nf = NULL; const char *volatile ok = "abc";
        if (!lstrcmpA(c, "control")) {
            int r1;
            if (!tc_iph_install()) return 3;
            tc_iph_count = 0;
            r1 = _scprintf(nf);
            return (r1 == -1 && tc_iph_count == 1) ? 77 : 100 + tc_iph_count;
        }
        if (!lstrcmpA(c, "nullfmt")) _scprintf(nf);
        else if (!lstrcmpA(c, "query")) return snprintf(NULL, 0, ok) == 3 ? 77 : 99;
        return 77;
    }
    TC_RUN(null_format_fatal); TC_RUN(length_query_survives); TC_RUN(control);
    return tc_end();
}
