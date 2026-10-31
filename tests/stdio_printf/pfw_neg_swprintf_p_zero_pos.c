/* TC
 * id:    pfw_neg_swprintf_p_zero_pos
 * area:  stdio_printf
 * funcs: 
 * procs: _swprintf_p _vswprintf_p __stdio_common_vswprintf_p
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (n$ with n starting at 1)
 * note:  PROBE: (1) a positional index of 0 (%0$d) is invalid -> invalid parameter handler, -1; (2) mixing positional (%1$d) and non-positional (%d) conversions in one format. Handler expected for both; exact errno not asserted.
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
typedef int (__cdecl *vsw_p_t)(unsigned __int64, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_p_t p_pp;
static int x_vswprintf_p(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_pp(0, b, n, f, NULL, ap); }
#define LOAD_P() ((p_pp = (vsw_p_t)tc_crt_proc("__stdio_common_vswprintf_p")) != NULL)
#else
typedef int (__cdecl *vswp_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vswp_t p_vswp;
static int x_vswprintf_p(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_vswp(b, n, f, ap); }
#define LOAD_P() ((p_vswp = (vswp_t)tc_crt_proc("_vswprintf_p")) != NULL)
#endif
static int x_swprintf_p(wchar_t *b, size_t n, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vswprintf_p(b, n, f, ap); va_end(ap); return r; }
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vnull_fmt;
static wchar_t * volatile vnull_buf;
static const wchar_t * volatile vzero = L"%0$d";
static const wchar_t * volatile vmix = L"%1$d %d";
TC_CASE(control) { wchar_t b[16]; int n = x_swprintf_p(b, 16, L"%2$d %1$d", 1, 2); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"2 1"); }
TC_CASE(zero_position_iph_ret_neg1) { wchar_t b[16]; int n; tc_iph_count = 0; n = x_swprintf_p(b, 16, vzero, 5); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(mix_positional_nonpositional) { wchar_t b[16]; int n; tc_iph_count = 0; n = x_swprintf_p(b, 16, vmix, 5, 6); CHECK_EQ_I(n, -1); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); CHECK(LOAD_P());
  TC_RUN(control); TC_RUN(zero_position_iph_ret_neg1); TC_RUN(mix_positional_nonpositional); return tc_end(); }
