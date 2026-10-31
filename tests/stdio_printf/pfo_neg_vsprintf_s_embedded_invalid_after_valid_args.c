/* TC
 * id:    pfo_neg_vsprintf_s_embedded_invalid_after_valid_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_s __stdio_common_vfprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, fprintf_s (invalid parameter handler invoked on NULL / too-small buffer)
 * note:  The handler must run exactly once per failing call; a successful call does not invoke it.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define TMP "tc_pfo_neg_vsprintf_s_embedded.tmp"
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cf(FILE *fp, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfprintf_s(0, fp, f, NULL, ap); va_end(ap); return r; }

TC_CASE(too_small)
{
    char b[8]; int r; tc_iph_count = 0;
    r = cs(b, 4, "%d%s", 12, "abcdef");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(file_null_format)
{
    const char *volatile f = NULL; FILE *fp = fopen(TMP, "w"); int r;
    if (!fp) TC_SKIP("cannot open temp");
    tc_iph_count = 0;
    r = cf(fp, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); DeleteFileA(TMP);
}
TC_CASE(null_buffer)
{
    char *volatile b = NULL; int r; tc_iph_count = 0;
    r = cs(b, 8, "%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(success_no_handler)
{
    char b[16]; int r; tc_iph_count = 0;
    r = cs(b, sizeof b, "%d", 42);
    CHECK_EQ_I(r, 2); CHECK_STR(b, "42"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(too_small); TC_RUN(file_null_format); TC_RUN(null_buffer); TC_RUN(success_no_handler);
    return tc_end();
}
