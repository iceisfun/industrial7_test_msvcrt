/* TC
 * id:    pfo_neg_vswprintf_p_null_and_small
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vswprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _swprintf_p, _vswprintf_p: NULL buffer/format -> invalid parameter handler, -1, EINVAL
 * note:  Direct calls to the UCRT worker with options 0. The too-small-buffer behaviour of the _p worker is not documented and lives in
 *        pfo_neg_probe_vswprintf_p_too_small.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
static wchar_t * volatile nbuf;
static const wchar_t * volatile nfmt;

TC_CASE(null_buffer)
{
    int r; tc_iph_count = 0; errno = 0;
    r = cw(nbuf, 16, L"%1$d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[16]; int r; b[0] = L'Z'; tc_iph_count = 0; errno = 0;
    r = cw(b, 16, nfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fits)
{
    wchar_t b[16]; int r; tc_iph_count = 0;
    r = cw(b, 16, L"%2$ls%1$ls", L"abc", L"def");
    CHECK_EQ_I(r, 6); CHECK_WSTR(b, L"defabc"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(fits);
    return tc_end();
}
