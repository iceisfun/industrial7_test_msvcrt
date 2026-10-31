/* TC
 * id:    pfu_neg_probe_snprintf_s_len_eq_size_buf0_erange
 * area:  stdio_printf
 * funcs: 
 * procs: _snprintf_s _snwprintf_s _vsnprintf_s _vsnwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s, _snwprintf_s, _vsnprintf_s, _vsnwprintf_s: count/_TRUNCATE semantics; buffer must hold chars plus the terminating NUL
 * note:  PROBES: after _snprintf_s family overflow with count==sizeOfBuffer==4 and 4 chars formatted: buffer[0]==0 and errno==ERANGE (not documented for _snprintf_s; sprintf_s doc only mentions buffer[0]). Return/handler are documented in pfu_neg_snprintf_s_len_eq_size.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
/* secure printf wrappers: ucrtbase has no exported sprintf_s & co (they are header inlines over __stdio_common_*);
 * older CRTs export them, resolved at run time. P_<name> is the uniform call syntax. */
#if TC_CRT_VER >= 140
int __cdecl __stdio_common_vsprintf_s(unsigned __int64, char *, size_t, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vswprintf_s(unsigned __int64, wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
int __cdecl __stdio_common_vsnprintf_s(unsigned __int64, char *, size_t, size_t, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vsnwprintf_s(unsigned __int64, wchar_t *, size_t, size_t, const wchar_t *, _locale_t, va_list);
int __cdecl __stdio_common_vfprintf_s(unsigned __int64, FILE *, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vfwprintf_s(unsigned __int64, FILE *, const wchar_t *, _locale_t, va_list);
static int U_v_snprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap) { return __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); }
static int P__snprintf_s(char *b, size_t n, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = U_v_snprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int U_v_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, va_list ap) { return __stdio_common_vsnwprintf_s(0, b, n, c, f, NULL, ap); }
static int P__snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = U_v_snwprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int P__vsnprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap) { return U_v_snprintf_s(b, n, c, f, ap); }
static int P__vsnwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, va_list ap) { return U_v_snwprintf_s(b, n, c, f, ap); }
#define LOAD_OR_DIE(n) ((void)0)
#else
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define P__snprintf_s(...) tc_p__snprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));
#define P__snwprintf_s(...) tc_p__snwprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));
#define P__vsnprintf_s(...) tc_p__vsnprintf_s(__VA_ARGS__)
TC_DECL_PROC(int, _vsnwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, va_list));
#define P__vsnwprintf_s(...) tc_p__vsnwprintf_s(__VA_ARGS__)
#define LOAD_OR_DIE(n) do { TC_LOAD_PROC(n); if (!tc_p_##n) { tc_write("TC FAIL " TC_TEST_ID "::__missing_" #n "\n"); return 1; } } while (0)
#endif
#define LOAD_ALL() do { LOAD_OR_DIE(_snprintf_s); LOAD_OR_DIE(_snwprintf_s); LOAD_OR_DIE(_vsnprintf_s); LOAD_OR_DIE(_vsnwprintf_s); } while (0)
static void fillc(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 'Z'; }
static void fillw(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }

static int cv_snprintf_s(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P__vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int cv_snwprintf_s(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = P__vsnwprintf_s(b, n, c, f, ap); va_end(ap); return r; }

static char nb[8]; static wchar_t wb[8];
/* run one call with size 4; buffers are pre-filled with 'Z'; result text lands in nb (narrowed) */
static int run(int kind, size_t count, int len)
{
    int r, i; fillc(nb, 8); fillw(wb, 8); errno = 0;
    if (kind == 0) r = P__snprintf_s(nb, 4, count, "%.*s", len, "abcd");
    else if (kind == 1) r = P__snwprintf_s(wb, 4, count, L"%.*ls", len, L"abcd");
    else if (kind == 2) r = cv_snprintf_s(nb, 4, count, "%.*s", len, "abcd");
    else r = cv_snwprintf_s(wb, 4, count, L"%.*ls", len, L"abcd");
    if (kind & 1) for (i = 0; i < 8; i++) nb[i] = (char)wb[i];
    nb[7] = 0;  /* sentinel: 'Z' fill beyond index 4 is never reached by a correct implementation */
    return r;
}
static void scenarios(int kind)
{
    int r;
    tc_iph_count = 0;
    r = run(kind, 4, 4);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(nb[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(narrow) { scenarios(0); }
TC_CASE(wide) { scenarios(1); }
TC_CASE(va_narrow) { scenarios(2); }
TC_CASE(va_wide) { scenarios(3); }
int main(void)
{
    tc_begin(); LOAD_ALL();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no_iph_api\n"); return 2; }
    TC_RUN(narrow); TC_RUN(wide); TC_RUN(va_narrow); TC_RUN(va_wide);
    return tc_end();
}
