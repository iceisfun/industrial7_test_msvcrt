/* TC
 * id:    pff_neg_snprintf_s_count_ge_size_probe
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s _snprintf_s __stdio_common_vsprintf_s __stdio_common_vsnprintf_s
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s (_TRUNCATE truncates and returns -1; count >= size otherwise invalid parameter)
 * note:  PROBE: _snprintf_s with count >= size and output not fitting: -1, invalid parameter handler once, errno ERANGE, buffer[0] emptied. Not spelled out on the page.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#if TC_CRT_VER >= 140
/* ucrtbase exports only the __stdio_common_* workers; call them as the SDK inline wrappers do */
typedef int (__cdecl *vs_t)(unsigned __int64, char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vn_t)(unsigned __int64, char *, size_t, size_t, const char *, void *, va_list);
static vs_t p_vs; static vn_t p_vn;
static int xsprintf_s(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_vs(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int xsnprintf_s(char *b, size_t n, size_t c, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_vn(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
#define LOADED() ((p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf_s")) != NULL && (p_vn = (vn_t)tc_crt_proc("__stdio_common_vsnprintf_s")) != NULL)
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define xsprintf_s tc_p_sprintf_s
#define xsnprintf_s tc_p__snprintf_s
#define LOADED() (TC_LOAD_PROC(sprintf_s) && TC_LOAD_PROC(_snprintf_s))
#endif
#include <stdlib.h>
static int p_iph;
TC_CASE(count_ge_size) { char b[8]; volatile double v = 1.5; int n, i; if (!p_iph) TC_SKIP("no iph"); for (i = 0; i < 8; i++) b[i] = 'Z';
    tc_iph_count = 0; errno = 0; n = xsnprintf_s(b, 4, 20, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); }
int main(void) { tc_begin(); p_iph = tc_iph_install() && LOADED(); TC_RUN(count_ge_size); return tc_end(); }
