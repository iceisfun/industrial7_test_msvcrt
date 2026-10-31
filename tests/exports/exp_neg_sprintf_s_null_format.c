/* TC
 * id:    exp_neg_sprintf_s_null_format
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: sprintf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s
 * note:  NULL buffer / NULL format: iph, -1, EINVAL.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *sps_t)(char *, size_t, const char *, ...);
static sps_t p_sps;

TC_CASE(null_format_minus1_einval)
{
    char b[8]; const char *volatile f = NULL;
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p_sps(b, sizeof b, f), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer_minus1_einval)
{
    char *volatile b = NULL;
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p_sps(b, 8, "x"), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_sps = (sps_t)tc_crt_proc("sprintf_s");
    if (!p_sps || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(null_format_minus1_einval); TC_RUN(null_buffer_minus1_einval);
    return tc_end();
}
