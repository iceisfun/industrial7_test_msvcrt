/* TC
 * id:    pfx_neg_setvbuf_bad_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: setvbuf fopen fclose
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs setvbuf (mode must be _IOFBF/_IOLBF/_IONBF; size must be 2..INT_MAX; otherwise EINVAL + invalid parameter handler, nonzero return)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile path = "tc_pfx_neg_setvbuf_a.tmp";
static volatile int badmode = 99;
static volatile size_t one = 1;
static char buf[512];

TC_CASE(bad_mode)
{
    FILE *f = fopen(path, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = setvbuf(f, buf, badmode, sizeof buf);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(size_one)
{
    FILE *f = fopen(path, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = setvbuf(f, buf, _IOFBF, one);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(valid_after_failure)
{
    FILE *f = fopen(path, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    setvbuf(f, buf, badmode, sizeof buf);
    r = setvbuf(f, buf, _IOFBF, sizeof buf);
    CHECK_EQ_I(r, 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(bad_mode); TC_RUN(size_one); TC_RUN(valid_after_failure);
    DeleteFileA(path);
    return tc_end();
}
