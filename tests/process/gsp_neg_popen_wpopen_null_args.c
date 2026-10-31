/* TC
 * id:    gsp_neg_popen_wpopen_null_args
 * area:  process
 * funcs: _popen _wpopen
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _popen, _wpopen (command and mode must be non-NULL; Parameter validation: EINVAL + handler, returns NULL)
 * note:  Unprefixed popen/wpopen. prc_neg_popen_null_args covers the underscore names.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile vcmd, *volatile vmode;
static const wchar_t *volatile wcmd, *volatile wmode;

static void expect_fail(FILE *f)
{
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (f) pclose(f);
}
#define PRE() do { if (!tc_iph_install()) TC_SKIP("no iph api"); tc_iph_count = 0; errno = 0; } while (0)
TC_CASE(popen_null_cmd)   { PRE(); vcmd = NULL; vmode = "r"; expect_fail(popen(vcmd, vmode)); }
TC_CASE(popen_null_mode)  { PRE(); vcmd = "cmd /c exit 0"; vmode = NULL; expect_fail(popen(vcmd, vmode)); }
TC_CASE(wpopen_null_cmd)  { PRE(); wcmd = NULL; wmode = L"r"; expect_fail(wpopen(wcmd, wmode)); }
TC_CASE(wpopen_null_mode) { PRE(); wcmd = L"cmd /c exit 0"; wmode = NULL; expect_fail(wpopen(wcmd, wmode)); }
int main(void)
{
    tc_begin();
    TC_RUN(popen_null_cmd); TC_RUN(popen_null_mode); TC_RUN(wpopen_null_cmd); TC_RUN(wpopen_null_mode);
    return tc_end();
}
