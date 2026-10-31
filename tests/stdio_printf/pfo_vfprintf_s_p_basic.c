/* TC
 * id:    pfo_vfprintf_s_p_basic
 * area:  stdio_printf
 * funcs: __stdio_common_vfprintf_s __stdio_common_vfprintf_p __stdio_common_vfwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf_s, _fprintf_p, fwprintf_s (return number of characters written)
 * note:  temp file tc_pfo_vfprintf_s_p_basic_*.tmp, removed. Wide variant: only the returned count is asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define FN "tc_pfo_vfprintf_s_p_basic_a.tmp"
static int cs(FILE *f, const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfprintf_s(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int cp(FILE *f, const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfprintf_p(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int cws(FILE *f, const wchar_t *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfwprintf_s(0, f, fmt, NULL, ap); va_end(ap); return r; }
static size_t slurp(char *buf, size_t n) { FILE *f = fopen(FN, "rb"); size_t r = 0; if (f) { r = fread(buf, 1, n, f); fclose(f); } return r; }

TC_CASE(s_variant)
{
    FILE *f = fopen(FN, "wb"); char b[64]; size_t n; int r;
    CHECK(f != NULL); if (!f) return;
    r = cs(f, "%d+%s", 2, "x"); fclose(f);
    CHECK_EQ_I(r, 3); n = slurp(b, 63); b[n] = 0; CHECK_STR(b, "2+x");
    remove(FN);
}
TC_CASE(p_variant)
{
    FILE *f = fopen(FN, "wb"); char b[64]; size_t n; int r;
    CHECK(f != NULL); if (!f) return;
    r = cp(f, "%2$s-%1$s-%2$s", "a", "b"); fclose(f);
    CHECK_EQ_I(r, 5); n = slurp(b, 63); b[n] = 0; CHECK_STR(b, "b-a-b");
    remove(FN);
}
TC_CASE(wide_s_count)
{
    FILE *f = fopen(FN, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    r = cws(f, L"%d/%ls", 10, L"ab"); fclose(f);
    CHECK_EQ_I(r, 5);
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(s_variant); TC_RUN(p_variant); TC_RUN(wide_s_count); return tc_end(); }
