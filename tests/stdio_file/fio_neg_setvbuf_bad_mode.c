/* TC
 * id:    fio_neg_setvbuf_bad_mode
 * area:  stdio_file
 * funcs: setvbuf fopen fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setvbuf; parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

static const char *fn = "tc_fio_neg_setvbuf_bad_mode_a.tmp";
static char b[1024];

TC_CASE(bad_mode)
{
    FILE *fp = fopen(fn, "wb"); volatile int r; volatile int m;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_install(); tc_iph_count = 0;
    errno = 0; m = 99;
    r = setvbuf(fp, b, m, sizeof b);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0; m = -1;
    r = setvbuf(fp, b, m, sizeof b);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 2);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(bad_mode); return tc_end(); }
