/* TC
 * id:    chr_neg_getc_putc_null_stream
 * area:  stdio_char
 * funcs: getc putc
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs getc, putc, Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *name, const char *data, size_t n)
{
    FILE *f = fopen(name, "wb");
    if (f) { if (n) fwrite(data, 1, n, f); fclose(f); }
}

TC_CASE(getc_null)
{
    int r;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    r = (getc)((FILE *)NULL);
    CHECK_EQ_I(r, EOF);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(putc_null)
{
    int r; volatile int c = 'a';
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    r = (putc)(c, (FILE *)NULL);
    CHECK_EQ_I(r, EOF);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(getc_null); TC_RUN(putc_null); return tc_end(); }
