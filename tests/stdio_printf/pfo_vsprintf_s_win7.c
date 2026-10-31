/* TC
 * id:    pfo_vsprintf_s_win7
 * area:  stdio_printf
 * funcs: vsprintf_s _vsnprintf_s _vscprintf
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs vsprintf_s, _vsnprintf_s ("_TRUNCATE: truncate, return -1"), _vscprintf ("number of chars, not including the NUL")
 * note:  msvcr80 and msvcrt excluded: mingw import libs lack some of these names
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int c_vsprintf_s(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int c_vsnprintf_s(char *b, size_t n, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int c_vscprintf(const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }

TC_CASE(vsprintf_s_len) { char b[16]; int r = c_vsprintf_s(b, 16, "%d:%s", 5, "ab"); CHECK_EQ_I(r, 4); CHECK_STR(b, "5:ab"); }
TC_CASE(trunc)
{
    char b[16]; int r; fill(b);
    r = c_vsnprintf_s(b, 4, (size_t)-1, "%s", "abcdef");   /* _TRUNCATE == (size_t)-1 */
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(count_fits) { char b[16]; int r = c_vsnprintf_s(b, 16, 10, "%s", "abcdef"); CHECK_EQ_I(r, 6); CHECK_STR(b, "abcdef"); }
TC_CASE(scprintf) { CHECK_EQ_I(c_vscprintf("%d-%s", 123, "ab"), 6); }
TC_CASE(scprintf_empty) { CHECK_EQ_I(c_vscprintf(""), 0); }
int main(void) { tc_begin(); TC_RUN(vsprintf_s_len); TC_RUN(trunc); TC_RUN(count_fits); TC_RUN(scprintf); TC_RUN(scprintf_empty); return tc_end(); }
