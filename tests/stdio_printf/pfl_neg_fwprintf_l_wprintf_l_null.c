/* TC
 * id:    pfl_neg_fwprintf_l_wprintf_l_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vfwprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, _fprintf_l, fwprintf, _fwprintf_l, _vfwprintf_l, printf, _wprintf_l ("Parameter validation": NULL stream or format -> EINVAL, -1)
 * note:  stdout is only used with a NULL format, which fails before any output or orientation change.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>

static _locale_t loc;
static FILE * volatile v_nofile;
static const wchar_t * volatile v_nowf;

static int vfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = pl_vfwprintf_l(f, fmt, loc, ap); va_end(ap); return r; }

TC_CASE(fwprintf_l_null_stream)
{
    int r; tc_iph_count = 0; errno = 0;
    r = pl_fwprintf_l(v_nofile, L"x", loc);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fwprintf_l_null_format)
{
    int r; tc_iph_count = 0; errno = 0;
    r = pl_fwprintf_l(stdout, v_nowf, loc);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wprintf_l_null_format)
{
    int r; tc_iph_count = 0; errno = 0;
    r = pl_wprintf_l(v_nowf, loc);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vfwprintf_l_null_stream)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vfw(v_nofile, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(fwprintf_l_null_stream); TC_RUN(fwprintf_l_null_format); TC_RUN(wprintf_l_null_format); TC_RUN(vfwprintf_l_null_stream);
    _free_locale(loc);
    return tc_end();
}
