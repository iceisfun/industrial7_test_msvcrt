/* TC
 * id:    pfi_int_snprintf_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_s __stdio_common_vsnprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: returns bytes written excl. NUL; always NUL-terminates when it succeeds
 */
#include "tc.h"
#include <stdio.h>

#include <stdarg.h>
#if TC_CRT_VER >= 140
typedef int (__cdecl *vsnprintf_s_t)(unsigned __int64, char *, size_t, size_t, const char *, void *, va_list);
static vsnprintf_s_t p_common;
static int xsnprintf_s(char *b, size_t n, size_t c, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_common(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
#define HAVE_SNPRINTF_S() ((p_common = (vsnprintf_s_t)tc_crt_proc("__stdio_common_vsnprintf_s")) != NULL)
#else
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define xsnprintf_s tc_p__snprintf_s
#define HAVE_SNPRINTF_S() (TC_LOAD_PROC(_snprintf_s) != NULL)
#endif

static int p_ok;
TC_CASE(count_lt_size) { char b[32]; int n; if (!p_ok) TC_SKIP("none"); n = xsnprintf_s(b, sizeof b, 10, "%d", -123456); CHECK_STR(b, "-123456"); CHECK_EQ_I(n, 7); }
TC_CASE(truncate_room) { char b[32]; int n; if (!p_ok) TC_SKIP("none"); n = xsnprintf_s(b, sizeof b, (size_t)-1, "%x|%u", 255u, 77u); CHECK_STR(b, "ff|77"); CHECK_EQ_I(n, 5); }
TC_CASE(nul) { char b[8]; int i; if (!p_ok) TC_SKIP("none"); for (i = 0; i < 8; i++) b[i] = 'Z'; xsnprintf_s(b, 8, 7, "%d", 12); CHECK(b[2] == 0); CHECK(b[3] == 'Z'); }
int main(void) { tc_begin(); p_ok = HAVE_SNPRINTF_S(); CHECK(p_ok); TC_RUN(count_lt_size); TC_RUN(truncate_room); TC_RUN(nul); return tc_end(); }
