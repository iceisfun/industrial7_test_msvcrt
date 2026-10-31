/* TC
 * id:    pff_neg_sprintf_s_float_null
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s __stdio_common_vsnprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s (NULL buffer/format: invalid parameter handler, returns -1, errno EINVAL)
 * note:  zero-size case lives in pff_neg_sprintf_s_zero_size (not spelled out in docs)
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

static int p_iph;
TC_CASE(null_buffer) { volatile double v = 1.5; int n; if (!p_iph) TC_SKIP("no iph"); tc_iph_count = 0; errno = 0;
    n = xsprintf_s(NULL, 16, "%f", v); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_format) { char b[16]; int n; if (!p_iph) TC_SKIP("no iph"); b[0] = 'Z'; tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, sizeof b, NULL); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); p_iph = tc_iph_install() && LOADED(); TC_RUN(null_buffer); TC_RUN(null_format); return tc_end(); }
