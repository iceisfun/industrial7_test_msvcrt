/* TC
 * id:    pfw_neg_snwprintf_s_truncate_errno_probe
 * area:  stdio_printf
 * funcs:
 * procs: _snwprintf_s _vsnwprintf_s __stdio_common_vsnwprintf_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs errno constants (STRUNCATE = 80); strncpy_s/_snprintf_s _TRUNCATE
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): _snwprintf_s / _vsnwprintf_s with count == _TRUNCATE return -1 on truncation, leave errno UNCHANGED (0), do not call the handler, and leave the
 *        truncated "123" + NUL. Docs state STRUNCATE for the strcpy_s family only; the old expectation errno == STRUNCATE was wrong here.
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
typedef int (__cdecl *snw_common_t)(unsigned __int64, wchar_t *, size_t, size_t, const wchar_t *, void *, va_list);
static snw_common_t p_snw_common;
static int x_vsnwprintf_s(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, va_list ap) { return p_snw_common(0, b, sz, cnt, f, NULL, ap); }
#define LOAD_SNWS() ((p_snw_common = (snw_common_t)tc_crt_proc("__stdio_common_vsnwprintf_s")) != NULL)
#else
typedef int (__cdecl *vsnws_t)(wchar_t *, size_t, size_t, const wchar_t *, va_list);
static vsnws_t p_vsnws;
static int x_vsnwprintf_s(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, va_list ap) { return p_vsnws(b, sz, cnt, f, ap); }
#define LOAD_SNWS() ((p_vsnws = (vsnws_t)tc_crt_proc("_vsnwprintf_s")) != NULL)
#endif
static int x_snwprintf_s(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vsnwprintf_s(b, sz, cnt, f, ap); va_end(ap); return r; }
#define TRUNC ((size_t)-1)
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
static int vsnws(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = x_vsnwprintf_s(b, sz, cnt, f, ap); va_end(ap); return r; }
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vnull_fmt;
static wchar_t * volatile vnull_buf;

TC_CASE(snwprintf_s_errno)
{
    wchar_t b[16]; int n; fill(b); errno = 0;
    n = x_snwprintf_s(b, 4, TRUNC, vfmt, 123456);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, 0); CHECK_WSTR(b, L"123");
}
TC_CASE(vsnwprintf_s_errno)
{
    wchar_t b[16]; int n; fill(b); errno = 0;
    n = vsnws(b, 4, TRUNC, vfmt, 123456);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, 0); CHECK_WSTR(b, L"123");
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); CHECK(LOAD_SNWS()); TC_RUN(snwprintf_s_errno); TC_RUN(vsnwprintf_s_errno); return tc_end(); }
