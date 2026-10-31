/* TC
 * id:    pff_neg_scprintf_invalid_float_spec
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf _vscprintf _scwprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf/_scwprintf/_vscprintf: NULL format -> invalid parameter handler, -1, errno EINVAL; C11 7.21.6.1 (format string ends at NUL)
 * note:  The invalid-conversion-after-a-float case is in pff_neg_scprintf_invalid_after_float_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int vcount(const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }

TC_CASE(scwprintf_null) { const wchar_t * volatile f = NULL; int r; tc_iph_count = 0; errno = 0; r = _scwprintf(f); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(vscprintf_null) { const char * volatile f = NULL; int r; tc_iph_count = 0; errno = 0; r = vcount(f, 1.0); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(scprintf_null_after_float) { const char * volatile f = NULL; int r; tc_iph_count = 0; errno = 0; r = _scprintf(f, 1.0); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(embedded_nul)
{
    volatile double v = 1.0; int r; tc_iph_count = 0;
    r = _scprintf("%.2f\0%f", v);
    CHECK_EQ_I(r, 4); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(scwprintf_null); TC_RUN(vscprintf_null); TC_RUN(scprintf_null_after_float); TC_RUN(embedded_nul);
    return tc_end();
}
