/* TC
 * id:    pfo_neg_probe_vswprintf_p_too_small
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vswprintf_p
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _swprintf_p (secure-family conventions; too-small behaviour not stated)
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): _p worker, options 0, buffer of 4 wchar_t and output "abcdef": returns -2 (the raw worker code for "buffer too small"; the
 *        inline _vsnprintf-style wrappers map it to -1), the buffer holds the truncated "abc" + NUL, errno unchanged, NO invalid-parameter call, nothing at index >= 4 written.
 *        The old guess (buffer[0]==0, ERANGE, handler) was wrong. Control: output that fits.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(control_fits)
{
    wchar_t b[16]; int r; r = cw(b, 16, L"%2$ls%1$ls", L"abc", L"def");
    CHECK_EQ_I(r, 6); CHECK_WSTR(b, L"defabc");
}
TC_CASE(too_small)
{
    wchar_t b[8]; int r, i; volatile size_t n = 4;
    for (i = 0; i < 8; i++) b[i] = L'Z';
    tc_iph_count = 0; errno = 0;
    r = cw(b, n, L"%1$ls%2$ls", L"abc", L"def");
    CHECK_EQ_I(r, -2); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    for (i = 4; i < 8; i++) CHECK_EQ_I(b[i], L'Z');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(control_fits); TC_RUN(too_small);
    return tc_end();
}
