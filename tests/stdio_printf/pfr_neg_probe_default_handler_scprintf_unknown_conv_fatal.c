/* TC
 * id:    pfr_neg_probe_default_handler_scprintf_unknown_conv_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf format specification; UCRT parameter validation
 * note:  PROBE: _scprintf("%y") (unknown conversion character) goes through the invalid-parameter path, so under the default
 *        handler the child terminates (exit code not 77, not 0); with a handler installed it returns -1 and calls it once.
 *        Child-process test.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(unknown_conv_fatal) { DWORD c = 0; CHECK(tc_spawn_self("unkconv", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
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
            return (r == -1 && tc_iph_count == 1) ? 77 : 100 + tc_iph_count;
        }
        _scprintf(bad);
        return 77;
    }
    TC_RUN(unknown_conv_fatal); TC_RUN(handler_variant);
    return tc_end();
}
