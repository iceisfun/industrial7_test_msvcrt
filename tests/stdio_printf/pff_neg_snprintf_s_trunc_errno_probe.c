/* TC
 * id:    pff_neg_snprintf_s_trunc_errno_probe
 * area:  stdio_printf
 * funcs:
 * procs: __stdio_common_vsnprintf_s _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s (errno on _TRUNCATE truncation)
 * note:  probed: truncation under _TRUNCATE sets errno to ERANGE (docs mention ERANGE for _s string functions truncation)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
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

TC_CASE(errno_erange) { char b[16]; volatile double v = 1.5; int n; if (!p_ok) TC_SKIP("none");
  errno = 0; n = xsnprintf_s(b, 5, (size_t)-1, "%f", v); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); }
int main(void) { tc_begin(); tc_iph_install(); p_ok = HAVE_X(); CHECK(p_ok); TC_RUN(errno_erange); return tc_end(); }
