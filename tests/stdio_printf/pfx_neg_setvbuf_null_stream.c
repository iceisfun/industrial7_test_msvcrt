/* TC
 * id:    pfx_neg_setvbuf_null_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: setvbuf fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs setvbuf (parameter validation: invalid stream -> invalid parameter handler, errno EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define FN "tc_pfx_neg_setvbuf_null_stream.tmp"
static FILE *volatile nul;

TC_CASE(null_fbf)
{
    int r;
    tc_iph_count = 0; errno = 0;
    r = setvbuf(nul, NULL, _IOFBF, 512);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_nbf)
{
    int r;
    tc_iph_count = 0; errno = 0;
    r = setvbuf(nul, NULL, _IONBF, 0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_after)
{
    FILE *f = fopen(FN, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    r = setvbuf(f, NULL, _IOFBF, 512);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f); DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph\n"); return 1; }
    TC_RUN(null_fbf); TC_RUN(null_nbf); TC_RUN(valid_after);
    return tc_end();
}
