/* TC
 * id:    pfx_neg_stdio_common_vfprintf_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vfprintf __stdio_common_vfwprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs fprintf/_fprintf_l/fwprintf (NULL stream or format -> invalid parameter handler, EINVAL, -1); in the Windows 10 SDK
 *        these are inline wrappers over __stdio_common_vfprintf / __stdio_common_vfwprintf
 * note:  conf is inferred: the worker itself is not documented; behaviour is inferred from the documented wrappers. UCRT counterpart of pfx_neg_fprintf_l_null_args: calls the exported worker directly with options 0 and a NULL locale.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define FN "tc_pfx_neg_stdio_common_vfprintf_null_args.tmp"
static FILE *volatile nul;
static const char *volatile nfmt;
static const wchar_t *volatile nwfmt;

static int vn(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfprintf(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int vw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vfwprintf(0, f, fmt, NULL, ap); va_end(ap); return r; }

#define EXPECT_BAD(call) do { int r_; tc_iph_count = 0; errno = 0; r_ = (call); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(null_stream) { EXPECT_BAD(vn(nul, "x")); }
TC_CASE(null_format)
{
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    EXPECT_BAD(vn(f, nfmt));
    fclose(f); DeleteFileA(FN);
}
TC_CASE(wide_null_stream) { EXPECT_BAD(vw(nul, L"x")); }
TC_CASE(wide_null_format)
{
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    EXPECT_BAD(vw(f, nwfmt));
    fclose(f); DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph\n"); return 1; }
    TC_RUN(null_stream); TC_RUN(null_format); TC_RUN(wide_null_stream); TC_RUN(wide_null_format);
    return tc_end();
}
