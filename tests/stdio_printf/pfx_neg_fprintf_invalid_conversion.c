/* TC
 * id:    pfx_neg_fprintf_invalid_conversion
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf format specification; "Parameter validation"
 * note:  Probing that an unknown conversion character after '%' (%y, %!) is rejected (-1, EINVAL, iph 1) rather than copied literally
 *        (pre-UCRT docs said unknown chars are copied); also that the stream stays usable. A trailing lone '%' is probed separately (pfx_probe_neg_trailing_percent).
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <errno.h>

#define TMP "tc_pfx_neg_fprintf_invalid_conversion_" TCV ".tmp"
static const char *volatile g_y = "%y";
static const char *volatile g_bang = "%!";
static const char *volatile g_pre = "ok%y";
static const char *volatile g_ok = "fine";

TC_CASE(percent_y)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, g_y, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(percent_bang)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, g_bang, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(stream_usable_after)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    r = fprintf(f, g_pre, 1);              /* prior output may or may not be emitted; only the failure is asserted */
    CHECK_EQ_I(r, -1);
    clearerr(f);
    errno = 0;
    r = fprintf(f, "%s", g_ok);
    CHECK_EQ_I(r, 4);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_I(fclose(f), 0);
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(percent_y); TC_RUN(percent_bang); TC_RUN(stream_usable_after);
    return tc_end();
}
