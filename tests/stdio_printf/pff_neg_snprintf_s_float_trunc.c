/* TC
 * id:    pff_neg_snprintf_s_float_trunc
 * area:  stdio_printf
 * funcs:
 * procs: __stdio_common_vsnprintf_s _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s: with _TRUNCATE the string is truncated, NUL-terminated, and -1 is returned
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#if TC_CRT_VER >= 140
typedef int (__cdecl *vsnprintf_s_t)(unsigned __int64, char *, size_t, size_t, const char *, void *, va_list);
static vsnprintf_s_t p_common;
static int xsnprintf_s(char *b, size_t n, size_t m, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_common(0, b, n, m, f, NULL, ap); va_end(ap); return r; }
#define HAVE_X() ((p_common = (vsnprintf_s_t)tc_crt_proc("__stdio_common_vsnprintf_s")) != NULL)
#else
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define xsnprintf_s tc_p__snprintf_s
#define HAVE_X() (TC_LOAD_PROC(_snprintf_s) != NULL)
#endif
static int p_ok;

static void fill(char *b) { int i; for (i = 0; i < 32; i++) b[i] = 'Z'; }
TC_CASE(truncate) { char b[32]; volatile double v = 1.5; int n; if (!p_ok) TC_SKIP("none"); fill(b); tc_iph_count = 0;
  n = xsnprintf_s(b, 5, (size_t)-1, "%f", v); CHECK_EQ_I(n, -1); CHECK_STR(b, "1.50"); CHECK(b[5] == 'Z'); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(fits_exactly) { char b[32]; volatile double v = 1.5; int n; if (!p_ok) TC_SKIP("none"); fill(b); tc_iph_count = 0;
  n = xsnprintf_s(b, 9, (size_t)-1, "%f", v); CHECK_EQ_I(n, 8); CHECK_STR(b, "1.500000"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(one_short) { char b[32]; volatile double v = 1.5; int n; if (!p_ok) TC_SKIP("none"); fill(b); tc_iph_count = 0;
  n = xsnprintf_s(b, 8, (size_t)-1, "%f", v); CHECK_EQ_I(n, -1); CHECK_STR(b, "1.50000"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); tc_iph_install(); p_ok = HAVE_X(); CHECK(p_ok);
  TC_RUN(truncate); TC_RUN(fits_exactly); TC_RUN(one_short); return tc_end(); }
