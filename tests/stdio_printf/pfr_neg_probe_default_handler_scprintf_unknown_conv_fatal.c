/* TC
 * id:    pfr_neg_probe_default_handler_scprintf_unknown_conv_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf format specification; UCRT parameter validation
 * note:  Observed on Windows 10 22H2: _scprintf("%y") (unknown conversion) is NOT an invalid-parameter path: it returns 1 and never calls the handler, so
 *        even with the default handler the child survives (77). With a handler installed the result is r == 1 and zero calls. Children call
 *        tc_iph_default() first (mingw-w64 installs a no-op handler at startup).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(unknown_conv_survives) { DWORD c = 0; CHECK(tc_spawn_self("unkconv", &c, 20000)); CHECK_EQ_U(c, 77); }
TC_CASE(handler_variant)    { DWORD c = 0; CHECK(tc_spawn_self("handler", &c, 20000)); CHECK_EQ_U(c, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        const char *volatile bad = "%y";
        if (!lstrcmpA(c, "handler")) {
            int r;
            if (!tc_iph_install()) return 3;
            tc_iph_count = 0;
            r = _scprintf(bad);
            return (r == 1 && tc_iph_count == 0) ? 77 : 100 + tc_iph_count;
        }
        tc_iph_default();
        _scprintf(bad);
        return 77;
    }
    TC_RUN(unknown_conv_survives); TC_RUN(handler_variant);
    return tc_end();
}
