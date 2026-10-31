/* TC
 * id:    fio_neg_setvbuf_size_small
 * area:  stdio_file
 * funcs: setvbuf fopen fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setvbuf: size must be >= 2 and < INT_MAX; parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

static const char *fn = "tc_fio_neg_setvbuf_size_small_a.tmp";
static char b[1024];

TC_CASE(bad_size)
{
    FILE *fp = fopen(fn, "wb"); volatile int r; volatile size_t sz;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_install(); tc_iph_count = 0;
    errno = 0; sz = 1;
    r = setvbuf(fp, b, _IOFBF, sz);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL);
    errno = 0; sz = 0;
    r = setvbuf(fp, b, _IOFBF, sz);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL);
    errno = 0; sz = (size_t)INT_MAX + 1u;
    r = setvbuf(fp, b, _IOFBF, sz);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 3);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(bad_size); return tc_end(); }
