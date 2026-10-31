/* TC
 * id:    pfw_neg_swprintf_c_truncate
 * area:  stdio_printf
 * funcs: 
 * procs: _swprintf_c _vswprintf_c __stdio_common_vswprintf
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs sprintf_c, _sprintf_c_l, swprintf_c: count-bounded form; returns a negative value when the output does not fit
 * note:  conf inferred: the count-bounded _swprintf_c is mirrored from the sprintf_s/_snprintf family docs; MS page coverage of the _c variants is thin. NUL termination after truncation is not asserted. ucrtbase: via the __stdio_common_vswprintf export. Only the first 3 characters are asserted for count 4.
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
typedef int (__cdecl *vsw_common_t)(unsigned __int64, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_common_t p_cc;
static int x_vswprintf_c(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_cc(0, b, n, f, NULL, ap); }
#define LOAD_C() ((p_cc = (vsw_common_t)tc_crt_proc("__stdio_common_vswprintf")) != NULL)
#else
typedef int (__cdecl *vswc_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vswc_t p_vswc;
static int x_vswprintf_c(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_vswc(b, n, f, ap); }
#define LOAD_C() ((p_vswc = (vswc_t)tc_crt_proc("_vswprintf_c")) != NULL)
#endif
static int x_swprintf_c(wchar_t *b, size_t n, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vswprintf_c(b, n, f, ap); va_end(ap); return r; }
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vnull_fmt;
static wchar_t * volatile vnull_buf;
static int vswc(wchar_t *b, size_t n, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vswprintf_c(b, n, f, ap); va_end(ap); return r; }

TC_CASE(control) { wchar_t b[16]; int n; fill(b); n = x_swprintf_c(b, 8, vfmt, 123); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"123"); }
TC_CASE(ret_neg1) { wchar_t b[16]; fill(b); CHECK(x_swprintf_c(b, 4, vfmt, 1234567) < 0); }
TC_CASE(prefix_written) { wchar_t b[16]; fill(b); x_swprintf_c(b, 4, vfmt, 1234567); CHECK(b[0] == L'1' && b[1] == L'2' && b[2] == L'3'); CHECK(b[4] == L'Z'); }
TC_CASE(vswprintf_c_same) { wchar_t b[16]; fill(b); CHECK(vswc(b, 4, vfmt, 1234567) < 0); CHECK(b[0] == L'1' && b[1] == L'2' && b[2] == L'3'); CHECK(b[4] == L'Z'); }
int main(void) { tc_begin(); CHECK(LOAD_C());
  TC_RUN(control); TC_RUN(ret_neg1); TC_RUN(prefix_written); TC_RUN(vswprintf_c_same); return tc_end(); }
