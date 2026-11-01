/* TC
 * id:    pfr_neg_probe_scprintf_bad_format_ucrt
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf _scwprintf _vscprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf format specification; UCRT parameter validation of format strings
 * note:  OBSERVED: the length-counting functions do not validate the format (like sprintf): "abc%" -> 3, "%y" -> 1, errno 0,
 *        no invalid-parameter handler call. Probed formats: lone trailing '%' after text, unknown conversion '%y'
 *        (narrow, wide and va_list forms).
 *        Observed on Win10 22H2 19045 ucrtbase: the non-_s printf family does not validate format strings (no handler call, no errno); the bad conversion char is copied through.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int vc(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }

TC_CASE(trailing_percent)
{
    const char *volatile f = "abc%"; int r;
    tc_iph_count = 0; errno = 0;
    r = _scprintf(f);
    CHECK_EQ_I(r, 3); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(unknown_conv)
{
    const char *volatile f = "%y"; int r;
    tc_iph_count = 0; errno = 0;
    r = _scprintf(f);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_unknown_conv)
{
    const wchar_t *volatile f = L"%y"; int r;
    tc_iph_count = 0; errno = 0;
    r = _scwprintf(f);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(vscprintf_unknown_conv)
{
    const char *volatile f = "%y"; int r;
    tc_iph_count = 0; errno = 0;
    r = vc(f, 1);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(trailing_percent); TC_RUN(unknown_conv); TC_RUN(wide_unknown_conv); TC_RUN(vscprintf_unknown_conv);
    return tc_end();
}
