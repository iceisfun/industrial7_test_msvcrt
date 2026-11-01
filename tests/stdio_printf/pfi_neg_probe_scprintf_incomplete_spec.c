/* TC
 * id:    pfi_neg_probe_scprintf_incomplete_spec
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf _vscprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _scprintf (-1 on error); printf format spec
 * note:  Observed on Windows 10 22H2: a format ending in an incomplete specification ("%5") is NOT rejected by the non-secure _scprintf/_vscprintf:
 *        they return 0 (nothing is printed) and do not call the invalid-parameter handler. Only the _s/_p families validate formats.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vs(const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static const char *volatile F2 = "%5";

TC_CASE(scprintf_incomplete) { int r; tc_iph_count = 0; r = _scprintf((const char *)F2); CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(vscprintf_incomplete) { int r; tc_iph_count = 0; r = vs((const char *)F2); CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(scprintf_incomplete); TC_RUN(vscprintf_incomplete);
    return tc_end();
}
