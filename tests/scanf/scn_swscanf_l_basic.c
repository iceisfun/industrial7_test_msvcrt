/* TC
 * id:    scn_swscanf_l_basic
 * area:  scanf
 * funcs: _swscanf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs swscanf, _swscanf_l
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
static int tcx_swscanf_l(const wchar_t *s, const wchar_t *f, _locale_t l, ...)
{ va_list ap; int r; va_start(ap, l); r = tcx_vw(s, (size_t)-1, f, l, ap); va_end(ap); return r; }
#undef _swscanf_l
#define _swscanf_l tcx_swscanf_l
#endif

static _locale_t L_;

TC_CASE(ints) { int a = 0, b = 0, r; r = _swscanf_l(L"12 -34", L"%d %d", L_, &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, -34); }
TC_CASE(lf) { double d = 0; int r; r = _swscanf_l(L"1.5", L"%lf", L_, &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.5); }
TC_CASE(ls) { wchar_t a[16]; int r; a[0] = 0; r = _swscanf_l(L"he中llo x", L"%ls", L_, a); CHECK_EQ_I(r, 1); CHECK_WSTR(a, L"he中" L"llo"); }
TC_CASE(mismatch_eof) { int a = 5, r; r = _swscanf_l(L"", L"%d", L_, &a); CHECK_EQ_I(r, EOF); r = _swscanf_l(L"zz", L"%d", L_, &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 5); }

int main(void)
{
    tc_begin();
    L_ = _create_locale(LC_ALL, "C");
    if (!L_) { tc_write("TC FAIL scn_swscanf_l_basic::create_locale\n"); return 1; }
    TC_RUN(ints); TC_RUN(lf); TC_RUN(ls); TC_RUN(mismatch_eof);
    _free_locale(L_);
    return tc_end();
}
