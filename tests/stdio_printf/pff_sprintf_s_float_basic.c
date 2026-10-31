/* TC
 * id:    pff_sprintf_s_float_basic
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s _snprintf_s __stdio_common_vsprintf_s __stdio_common_vsnprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_s_l, _snprintf_s
 * note:  msvcr80 excluded (mingw import lib lacks the symbol). The count<needed return value is covered by the negative tests.
 */
#include "tc.h"
#include <stdio.h>

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

TC_CASE(exact_fit)  { char b[16]; volatile double v = 1.5; int n = xsprintf_s(b, 9, "%f", v); CHECK_EQ_I(n, 8); CHECK_STR(b, "1.500000"); }
TC_CASE(e_and_g)
{
    char b[32]; volatile double v = 1.5, w = 0.0001; int n;
    n = xsprintf_s(b, sizeof b, "%.3f|%g", v, w); CHECK_EQ_I(n, 12); CHECK_STR(b, "1.500|0.0001");
    n = xsprintf_s(b, sizeof b, "%.0e", v); CHECK(n >= 5); CHECK(b[0] == '2' && b[1] == 'e' && b[2] == '+');
}
TC_CASE(snprintf_s_fits) { char b[16]; volatile double v = 1.5; int n; n = xsnprintf_s(b, 16, 8, "%f", v); CHECK_EQ_I(n, 8); CHECK_STR(b, "1.500000"); }
TC_CASE(snprintf_s_count_prefix)
{
    char b[16]; volatile double v = 1.5; int i;
   
    for (i = 0; i < 16; i++) b[i] = 'Z';
    (void)xsnprintf_s(b, 16, 3, "%f", v);
    CHECK(b[0] == '1' && b[1] == '.' && b[2] == '5'); CHECK_EQ_I(b[3], 0);
}
int main(void) { tc_begin(); CHECK(LOADED()); TC_RUN(exact_fit); TC_RUN(e_and_g); TC_RUN(snprintf_s_fits); TC_RUN(snprintf_s_count_prefix); return tc_end(); }
