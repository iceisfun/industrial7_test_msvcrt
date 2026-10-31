/* TC
 * id:    scn_snwscanf_basic
 * area:  scanf
 * funcs: _snwscanf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf, _snwscanf (count is in characters, i.e. wchar_t units)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#ifdef _UCRT
/* mingw headers do not give a usable declaration for these on ucrt; call the ucrt entry point the way MS headers do */
#include <stdarg.h>
static int tcx_vw(const wchar_t *s, size_t n, const wchar_t *f, _locale_t l, va_list ap)
{ return __stdio_common_vswscanf(_CRT_INTERNAL_LOCAL_SCANF_OPTIONS, s, n, f, l, ap); }
static int tcx_snwscanf(const wchar_t *s, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = tcx_vw(s, n, f, NULL, ap); va_end(ap); return r; }
#undef _snwscanf
#define _snwscanf tcx_snwscanf
#endif

static volatile size_t cnt;

TC_CASE(int_truncated) { int v = 0, r; cnt = 3; r = _snwscanf(L"12345", cnt, L"%d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 123); }
TC_CASE(ls_truncated) { wchar_t b[16]; int r; b[0] = 0; cnt = 3; r = _snwscanf(L"abcdef", cnt, L"%ls", b); CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"abc"); }
TC_CASE(not_terminated) { wchar_t src[8] = {L'a',L'b',L'c',L'1',L'2',L'3',L'x',L'y'}; wchar_t w[16]; int n = 0, r; w[0] = 0; cnt = 6; r = _snwscanf(src, cnt, L"%l[a-z]%d", w, &n); CHECK_EQ_I(r, 2); CHECK_WSTR(w, L"abc"); CHECK_EQ_I(n, 123); }
TC_CASE(lc) { wchar_t c[2] = {0, 0}; int r; cnt = 1; r = _snwscanf(L"中x", cnt, L"%lc", &c[0]); CHECK_EQ_I(r, 1); CHECK_EQ_U(c[0], 0x4e2d); }
TC_CASE(lc_count_one_second_missing) { wchar_t c[2] = {L'-', L'-'}; int r; cnt = 1; r = _snwscanf(L"ab", cnt, L"%lc%lc", &c[0], &c[1]); CHECK_EQ_I(r, 1); CHECK_EQ_U(c[0], L'a'); CHECK_EQ_U(c[1], L'-'); }

int main(void) { tc_begin(); TC_RUN(int_truncated); TC_RUN(ls_truncated); TC_RUN(not_terminated); TC_RUN(lc); TC_RUN(lc_count_one_second_missing); return tc_end(); }
