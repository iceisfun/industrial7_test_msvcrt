/* TC
 * id:    chr_neg_fgets_null_stream
 * area:  stdio_char
 * funcs: fgets
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fgets, Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *name, const char *data, size_t n)
{
    FILE *f = fopen(name, "wb");
    if (f) { if (n) fwrite(data, 1, n, f); fclose(f); }
}

TC_CASE(null_stream)
{
    char buf[8]; char *r; int i, same = 1;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    for (i = 0; i < 8; i++) buf[i] = 'Q';
    tc_iph_count = 0; errno = 0;
    r = fgets(buf, 8, (FILE *)NULL);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 8; i++) if (buf[i] != 'Q') same = 0;
    CHECK(same);
}
int main(void) { tc_begin(); TC_RUN(null_stream); return tc_end(); }
