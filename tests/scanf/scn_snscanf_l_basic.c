/* TC
 * id:    scn_snscanf_l_basic
 * area:  scanf
 * funcs: _snscanf_l _snwscanf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf, _snscanf_l, _snwscanf_l
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#ifdef _UCRT
/* mingw headers do not give a usable declaration for these on ucrt; call the ucrt entry point the way MS headers do */
#include <stdarg.h>
static int tcx_vw(const wchar_t *s, size_t n, const wchar_t *f, _locale_t l, va_list ap)
{ return __stdio_common_vswscanf(_CRT_INTERNAL_LOCAL_SCANF_OPTIONS, s, n, f, l, ap); }
static int tcx_snwscanf(const wchar_t *s, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = tcx_vw(s, n, f, NULL, ap); va_end(ap); return r; }
#undef _snwscanf
#define _snwscanf tcx_snwscanf
static int tcx_snwscanf_l(const wchar_t *s, size_t n, const wchar_t *f, _locale_t l, ...)
{ va_list ap; int r; va_start(ap, l); r = tcx_vw(s, n, f, l, ap); va_end(ap); return r; }
#undef _snwscanf_l
#define _snwscanf_l tcx_snwscanf_l
#endif

static _locale_t L_;
static volatile size_t cnt;

TC_CASE(narrow_limit) { int v = 0, r; cnt = 3; r = _snscanf_l("12345", cnt, "%d", L_, &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 123); }
TC_CASE(narrow_two) { int a = 0, b = 0, r; cnt = 4; r = _snscanf_l("12 345", cnt, "%d %d", L_, &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 3); }
TC_CASE(narrow_eof) { int v = 5, r; cnt = 2; r = _snscanf_l("  12", cnt, "%d", L_, &v); CHECK_EQ_I(r, EOF); CHECK_EQ_I(v, 5); }
TC_CASE(wide_limit) { int v = 0, r; cnt = 3; r = _snwscanf_l(L"12345", cnt, L"%d", L_, &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 123); }
TC_CASE(wide_str) { wchar_t b[16]; int r; b[0] = 0; cnt = 4; r = _snwscanf_l(L"abcdefg", cnt, L"%ls", L_, b); CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"abcd"); }

int main(void)
{
    tc_begin();
    L_ = _create_locale(LC_ALL, "C");
    if (!L_) { tc_write("TC FAIL scn_snscanf_l_basic::create_locale\n"); return 1; }
    TC_RUN(narrow_limit); TC_RUN(narrow_two); TC_RUN(narrow_eof); TC_RUN(wide_limit); TC_RUN(wide_str);
    _free_locale(L_);
    return tc_end();
}
