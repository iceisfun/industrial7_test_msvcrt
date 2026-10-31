/* TC
 * id:    exp_neg_snprintf_s_truncate
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _snprintf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf_s
 * note:  _TRUNCATE truncates, returns -1, no iph. errno not asserted.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *sns_t)(char *, size_t, size_t, const char *, ...);
static sns_t p_sns;
#define TC_TRUNC ((size_t)-1)

TC_CASE(truncate_returns_minus1)
{ char b[4]; tc_iph_count = 0; CHECK_EQ_I(p_sns(b, sizeof b, TC_TRUNC, "%s", "hello"), -1); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(dest_hel)
{ char b[4]; memset(b, 'z', sizeof b); p_sns(b, sizeof b, TC_TRUNC, "%s", "hello"); CHECK_STR(b, "hel"); }
TC_CASE(count_gt_size_no_truncate_flag)
{
    char b[4]; tc_iph_count = 0;
    CHECK_EQ_I(p_sns(b, sizeof b, 100, "%s", "hello"), -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_sns = (sns_t)tc_crt_proc("_snprintf_s");
    if (!p_sns || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(truncate_returns_minus1); TC_RUN(dest_hel); TC_RUN(count_gt_size_no_truncate_flag);
    return tc_end();
}
