/* TC
 * id:    gsf_neg_clearerr_s_null
 * area:  stdio_file
 * funcs: clearerr_s fopen fclose
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs clearerr_s: EINVAL if stream is NULL; Parameter validation
 * note:  NULL passed through a volatile so the call cannot be folded.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static void mk(const char *n, const char *d, size_t len)
{
    FILE *f = fopen(n, "wb");
    if (f) { fwrite(d, 1, len, f); fclose(f); }
}
static const char *fn = "tc_gsf_neg_clearerr_s_null_" TC_S1(TC_CRT_VER) ".tmp";
static FILE *volatile nullfp;

TC_CASE(null_stream)
{
    int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = clearerr_s(nullfp);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(repeat_after_failure)
{
    FILE *fp; mk(fn, "ab", 2);
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    CHECK_EQ_I(clearerr_s(nullfp), EINVAL);
    fp = fopen(fn, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(clearerr_s(fp), 0);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
}
int main(void) { tc_begin(); TC_RUN(null_stream); TC_RUN(repeat_after_failure); return tc_end(); }
