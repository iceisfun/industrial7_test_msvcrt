/* TC
 * id:    pff_neg_probe_vsprintf_s_null_fmt_buf0
 * area:  stdio_printf
 * kind:  negative
 * funcs: vsprintf_s _vsnprintf_s vswprintf_s swprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs vsprintf_s: the *_s functions write an empty string to a valid buffer on failure
 * note:  PROBES: with a valid buffer (size 16) and a NULL format, after the failing *_s call buf[0]==0 (empty string stored).
 *        Return/errno/IPH for these are asserted in pff_neg_vsprintf_s_null_args.
 *        Also probes that swprintf_s with a zero buffer size fails (-1, IPH) without touching the buffer.
 *        Win10 22H2 (observed): a NULL format returns -1 and leaves the buffer UNTOUCHED (b[0] stays 'x'); the previous expectation b[0]==0 was a guess.
 *        Raw evidence: b[0] == 0 got=120 want=0 (all four cases).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static int c_vs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int c_vsn(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int c_vsw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf_s(b, n, f, ap); va_end(ap); return r; }

TC_CASE(vsprintf_s_buf0)
{
    char b[16]; const char * volatile nf = NULL; int r;
    memset(b, 'x', sizeof b);
    r = c_vs(b, 16, nf, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 'x');
}
TC_CASE(vsnprintf_s_buf0)
{
    char b[16]; const char * volatile nf = NULL; int r;
    memset(b, 'x', sizeof b);
    r = c_vsn(b, 16, _TRUNCATE, nf, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 'x');
}
TC_CASE(vswprintf_s_buf0)
{
    wchar_t b[16]; const wchar_t * volatile nf = NULL; int r, i;
    for (i = 0; i < 16; i++) b[i] = L'x';
    r = c_vsw(b, 16, nf, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 'x');
}
TC_CASE(swprintf_s_null_fmt_buf0)
{
    wchar_t b[10]; const wchar_t * volatile nf = NULL; int r, i;
    for (i = 0; i < 10; i++) b[i] = L'x';
    r = swprintf_s(b, 10, nf);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 'x');
}
TC_CASE(swprintf_s_size0)
{
    wchar_t b[10]; volatile size_t z = 0; int r, i;
    for (i = 0; i < 10; i++) b[i] = L'x';
    tc_iph_count = 0; errno = 0;
    r = swprintf_s(b, z, L"%f", 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 10; i++) CHECK(b[i] == L'x');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(vsprintf_s_buf0); TC_RUN(vsnprintf_s_buf0); TC_RUN(vswprintf_s_buf0); TC_RUN(swprintf_s_null_fmt_buf0); TC_RUN(swprintf_s_size0);
    return tc_end();
}
