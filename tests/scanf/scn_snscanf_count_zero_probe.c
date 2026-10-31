/* TC
 * id:    scn_snscanf_count_zero_probe
 * area:  scanf
 * funcs: _snscanf _snwscanf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snscanf
 * note:  PROBE: count==0 means no input is available, so EOF is expected and the target untouched.
 *        It is not known whether any CRT raises the invalid-parameter handler; handler is installed (where
 *        present) so it cannot be fatal, and the iph count is deliberately NOT asserted.
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

TC_CASE(narrow) { int x = 5, r; cnt = 0; r = _snscanf("12", cnt, "%d", &x); CHECK_EQ_I(r, EOF); CHECK_EQ_I(x, 5); }
TC_CASE(wide) { int x = 5, r; cnt = 0; r = _snwscanf(L"12", cnt, L"%d", &x); CHECK_EQ_I(r, EOF); CHECK_EQ_I(x, 5); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
