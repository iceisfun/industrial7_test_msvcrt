/* TC
 * id:    pfo_vfprintf_basic
 * area:  stdio_printf
 * funcs: __stdio_common_vfprintf __stdio_common_vfwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, fwprintf (return count of chars written); C11 7.21.6.1
 * note:  temp file tc_pfo_vfprintf_basic_*.tmp, removed. Wide case uses a default text-mode ("w") file so wide output is converted to narrow; no newlines are written, so text/binary are byte-identical for ASCII.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define FN "tc_pfo_vfprintf_basic_a.tmp"
static int callf(FILE *f, const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfprintf(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int callfw(FILE *f, const wchar_t *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfwprintf(0, f, fmt, NULL, ap); va_end(ap); return r; }
static size_t slurp(char *buf, size_t n) { FILE *f = fopen(FN, "rb"); size_t r = 0; if (f) { r = fread(buf, 1, n, f); fclose(f); } return r; }

TC_CASE(narrow)
{
    FILE *f = fopen(FN, "wb"); char b[64]; size_t n; int r;
    CHECK(f != NULL); if (!f) return;
    r = callf(f, "n=%d s=%s", 42, "hi");
    fclose(f);
    CHECK_EQ_I(r, 9);
    n = slurp(b, 63); b[n] = 0;
    CHECK_EQ_U(n, 9); CHECK_STR(b, "n=42 s=hi");
    remove(FN);
}
TC_CASE(wide_on_narrow_file)
{
    FILE *f = fopen(FN, "w"); char b[64]; size_t n; int r;
    CHECK(f != NULL); if (!f) return;
    r = callfw(f, L"w=%d %ls", 7, L"ok");
    fclose(f);
    CHECK_EQ_I(r, 6);
    n = slurp(b, 63); b[n] = 0;
    CHECK_EQ_U(n, 6); CHECK_STR(b, "w=7 ok");
    remove(FN);
}
TC_CASE(count_accumulates)
{
    FILE *f = fopen(FN, "wb"); char b[64]; size_t n; int r1, r2;
    CHECK(f != NULL); if (!f) return;
    r1 = callf(f, "%s", "abc"); r2 = callf(f, "%05d", 12);
    fclose(f);
    CHECK_EQ_I(r1, 3); CHECK_EQ_I(r2, 5);
    n = slurp(b, 63); b[n] = 0; CHECK_STR(b, "abc00012");
    remove(FN);
}
int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide_on_narrow_file); TC_RUN(count_accumulates); return tc_end(); }
