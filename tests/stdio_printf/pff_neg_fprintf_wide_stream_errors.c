/* TC
 * id:    pff_neg_fprintf_wide_stream_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf fwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fwprintf, fwprintf_s: stream or format NULL -> invalid parameter handler, -1, errno EINVAL
 * note:  Read-only stream behaviour is probed in pff_neg_probe_fwprintf_readonly_stream.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define TMP "tc_pff_neg_fprintf_wide_stream_errors.tmp"
#define EXPECT_FAIL(expr) do { int r_; tc_iph_count = 0; errno = 0; r_ = (expr); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(fwprintf_null_stream)
{
    FILE * volatile s = NULL; volatile double v = 1.0;
    EXPECT_FAIL(fwprintf(s, L"%f", v));
}
TC_CASE(fwprintf_null_format)
{
    FILE *f = fopen(TMP, "w"); const wchar_t * volatile nf = NULL;
    CHECK(f != NULL); if (!f) return;
    EXPECT_FAIL(fwprintf(f, nf, 1.0));
    fclose(f);
}
TC_CASE(fwprintf_s_null_stream)
{
    FILE * volatile s = NULL; volatile double v = 1.0;
    EXPECT_FAIL(fwprintf_s(s, L"%f", v));
}
TC_CASE(fwprintf_s_null_format)
{
    FILE *f = fopen(TMP, "w"); const wchar_t * volatile nf = NULL;
    CHECK(f != NULL); if (!f) return;
    EXPECT_FAIL(fwprintf_s(f, nf, 1.0));
    fclose(f);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(fwprintf_null_stream); TC_RUN(fwprintf_null_format); TC_RUN(fwprintf_s_null_stream); TC_RUN(fwprintf_s_null_format);
    remove(TMP);
    return tc_end();
}
