/* TC
 * id:    pfi_neg_fprintf_int_null_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, vfprintf: NULL stream or format -> invalid parameter handler, EINVAL, -1
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static FILE *volatile nullf = NULL;
static const char *volatile nullfmt = NULL;
static int vcall(FILE *f, const char *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = vfprintf(f, fmt, ap); va_end(ap); return r; }

TC_CASE(fprintf_null_stream)
{
    int n; errno = 0; tc_iph_count = 0;
    n = fprintf(nullf, "%d", 5);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vfprintf_null_stream)
{
    int n; errno = 0; tc_iph_count = 0;
    n = vcall(nullf, "%d", 5);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format_valid_stream)
{
    const char *name = "tc_pfi_neg_fprintf_int_null_stream.tmp";
    FILE *f = fopen(name, "wb"); int n; long sz;
    CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    n = fprintf(f, nullfmt);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fflush(f); fseek(f, 0, SEEK_END); sz = ftell(f);
    CHECK_EQ_I(sz, 0);
    fclose(f); remove(name);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(fprintf_null_stream); TC_RUN(vfprintf_null_stream); TC_RUN(null_format_valid_stream);
    return tc_end();
}
