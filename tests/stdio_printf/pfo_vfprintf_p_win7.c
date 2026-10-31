/* TC
 * id:    pfo_vfprintf_p_win7
 * area:  stdio_printf
 * funcs: _vfprintf_p
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _printf_p, _vfprintf_p (positional parameters, returns number of characters written)
 * note:  temp file tc_pfo_vfprintf_p_win7_*.tmp, removed
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define FN "tc_pfo_vfprintf_p_win7_a.tmp"
static int cp(FILE *f, const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = _vfprintf_p(f, fmt, ap); va_end(ap); return r; }

TC_CASE(reorder)
{
    FILE *f = fopen(FN, "wb"); char b[64]; size_t n = 0; int r;
    CHECK(f != NULL); if (!f) return;
    r = cp(f, "%2$s %1$s", "world", "hello"); fclose(f);
    CHECK_EQ_I(r, 11);
    f = fopen(FN, "rb"); if (f) { n = fread(b, 1, 63, f); fclose(f); } b[n] = 0;
    CHECK_STR(b, "hello world");
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(reorder); return tc_end(); }
