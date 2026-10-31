/* TC
 * id:    pfi_int_sprintf_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, sprintf_s_l: returns number of bytes written excluding the NUL
 */
#include "tc.h"
#include <stdio.h>

#include <stdarg.h>
#if TC_CRT_VER >= 140
/* ucrtbase only has these inline (stdio.h) over __stdio_common_vsprintf_s; resolve the real export. */
typedef int (__cdecl *vsprintf_s_t)(unsigned __int64, char *, size_t, const char *, void *, va_list);
static vsprintf_s_t p_common;
static int xsprintf_s(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_common(0, b, n, f, NULL, ap); va_end(ap); return r; }
#define HAVE_SPRINTF_S() ((p_common = (vsprintf_s_t)tc_crt_proc("__stdio_common_vsprintf_s")) != NULL)
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define xsprintf_s tc_p_sprintf_s
#define HAVE_SPRINTF_S() (TC_LOAD_PROC(sprintf_s) != NULL)
#endif

static int p_ok;
TC_CASE(conv) { char b[64]; int n; if (!p_ok) TC_SKIP("none"); n = xsprintf_s(b, sizeof b, "%d %u %x %o", -9, 9u, 255u, 8u); CHECK_STR(b, "-9 9 ff 10"); CHECK_EQ_I(n, 10); }
TC_CASE(flags) { char b[64]; if (!p_ok) TC_SKIP("none"); xsprintf_s(b, sizeof b, "[%-5d][%+d][%05d][%.4d][%#x]", 1, 2, 3, 4, 255); CHECK_STR(b, "[1    ][+2][00003][0004][0xff]"); }
TC_CASE(exact_fit) { char b[8]; int n; int i; if (!p_ok) TC_SKIP("none"); for (i = 0; i < 8; i++) b[i] = 'Z';
  n = xsprintf_s(b, 6, "%d", 12345); CHECK_EQ_I(n, 5); CHECK_STR(b, "12345"); CHECK(b[6] == 'Z'); }
TC_CASE(prec_zero) { char b[16]; int n; if (!p_ok) TC_SKIP("none"); n = xsprintf_s(b, sizeof b, "[%.0d]", 0); CHECK_STR(b, "[]"); CHECK_EQ_I(n, 2); }
int main(void) { tc_begin(); p_ok = HAVE_SPRINTF_S(); CHECK(p_ok); TC_RUN(conv); TC_RUN(flags); TC_RUN(exact_fit); TC_RUN(prec_zero); return tc_end(); }
