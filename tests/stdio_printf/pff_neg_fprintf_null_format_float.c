/* TC
 * id:    pff_neg_fprintf_null_format_float
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf vfprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, _fprintf_l, vfprintf: format NULL -> invalid parameter handler, returns negative, errno EINVAL
 * note:  Valid stream, NULL format. Also checks that the stream is not poisoned and a following float print still works (non-tie value).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define TMP "tc_pff_neg_fprintf_null_format_float.tmp"

static int call_vf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = vfprintf(f, fmt, ap); va_end(ap); return r; }

TC_CASE(null_format)
{
    FILE *f = fopen(TMP, "w+"); const char * volatile nf = NULL; int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = fprintf(f, nf, 1.5);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(ftell(f), 0);
    fclose(f);
}
TC_CASE(vf_null_format)
{
    FILE *f = fopen(TMP, "w+"); const char * volatile nf = NULL; int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = call_vf(f, nf, 1.5);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(ftell(f), 0);
    fclose(f);
}
TC_CASE(nothing_written_and_not_poisoned)
{
    FILE *f = fopen(TMP, "w+"); const char * volatile nf = NULL; volatile double v = 3.14159; char b[32] = {0}; int r;
    CHECK(f != NULL); if (!f) return;
    fprintf(f, nf, 1.5); call_vf(f, nf, 1.5);
    CHECK_EQ_I(ftell(f), 0);
    r = fprintf(f, "%.2f", v);
    CHECK_EQ_I(r, 4);
    fflush(f); rewind(f);
    CHECK(fread(b, 1, 31, f) == 4);
    CHECK_STR(b, "3.14");
    CHECK_EQ_I(ferror(f), 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_format); TC_RUN(vf_null_format); TC_RUN(nothing_written_and_not_poisoned);
    remove(TMP);
    return tc_end();
}
