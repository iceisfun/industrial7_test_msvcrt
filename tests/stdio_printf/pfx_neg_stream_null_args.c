/* TC
 * id:    pfx_neg_stream_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: fclose ftell fseek fflush ferror
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fclose, ftell, fseek, ferror (NULL stream -> invalid parameter handler); fflush (NULL flushes all streams)
 * note:  ferror(NULL) return value is not asserted, only that the handler ran.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE *volatile nf = NULL;

TC_CASE(fclose_null) { int r; tc_iph_count = 0; errno = 0; r = fclose(nf); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ftell_null)  { long r; tc_iph_count = 0; errno = 0; r = ftell(nf); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fseek_null)  { int r; tc_iph_count = 0; errno = 0; r = fseek(nf, 0, SEEK_SET); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ferror_null) { tc_iph_count = 0; (void)ferror(nf); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fflush_null_valid) { int r; tc_iph_count = 0; r = fflush(nf); CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(fclose_null); TC_RUN(ftell_null); TC_RUN(fseek_null); TC_RUN(ferror_null); TC_RUN(fflush_null_valid);
    return tc_end();
}
