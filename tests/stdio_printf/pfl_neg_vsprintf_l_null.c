/* TC
 * id:    pfl_neg_vsprintf_l_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_l _vsnprintf_l _vscprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", vsprintf/_vsprintf_l, _vsnprintf_l, _vscprintf_l (NULL pointers: EINVAL, return -1)
 * note:  NULL buffer with count 0 must not invoke the handler (UCRT only; other CRTs skip). Its return value is NOT asserted: the legacy
 *        _vsnprintf family reports -1, not the needed length, when output does not fit.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <locale.h>

static _locale_t loc;

static int vs(char *b, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsprintf_l(b, f, loc, ap); va_end(ap); return r; }
static int vn(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_l(b, n, f, loc, ap); va_end(ap); return r; }
static int vc(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vscprintf_l(f, loc, ap); va_end(ap); return r; }

TC_CASE(vsprintf_l_null_fmt)
{
    char b[8]; const char *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0; b[0] = 'Q';
    r = vs(b, f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(vsprintf_l_null_buf)
{
    char *volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vs(b, "abc", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(vsnprintf_l_null_buf_count_nonzero)
{
    char *volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vn(b, 8, "abc", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(vscprintf_l_null_fmt)
{
    const char *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vc(f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(vsnprintf_l_null_buf_count_zero_no_iph_ucrt_only)
{
#if TC_CRT_VER >= 140
    char *volatile b = NULL; int r;
    tc_iph_count = 0;
    r = vn(b, 0, "abc%d", 12);
    (void)r;  /* return value deliberately not asserted: legacy _vsnprintf returns -1 when output does not fit (see pfl_probe_snprintf_l_count_zero) */
    CHECK_EQ_I(tc_iph_count, 0);
#else
    TC_SKIP("NULL buffer with count 0 documented only for the UCRT");
#endif
}

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(vsprintf_l_null_fmt); TC_RUN(vsprintf_l_null_buf); TC_RUN(vsnprintf_l_null_buf_count_nonzero);
    TC_RUN(vscprintf_l_null_fmt); TC_RUN(vsnprintf_l_null_buf_count_zero_no_iph_ucrt_only);
    _free_locale(loc);
    return tc_end();
}
