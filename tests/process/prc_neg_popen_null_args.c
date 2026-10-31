/* TC
 * id:    prc_neg_popen_null_args
 * area:  process
 * funcs: _popen _wpopen
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _popen (errno EINVAL on invalid parameter); NULL validation per CRT source behaviour, not explicit in docs
 * note:  NULL command/mode -> iph + NULL + EINVAL is the well-known CRT parameter validation (>=80) but docs do not spell it out; hence inferred.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char *volatile vcmd;
static const char *volatile vmode;
static const wchar_t *volatile wcmd;
static const wchar_t *volatile wmode;
static void expect_fail(FILE *f)
{
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (f) _pclose(f);
}
#define PRE() do { if (!tc_iph_install()) TC_SKIP("no iph api"); tc_iph_count = 0; errno = 0; } while (0)
TC_CASE(null_cmd)   { PRE(); vcmd = NULL; vmode = "r"; expect_fail(_popen(vcmd, vmode)); }
TC_CASE(null_mode)  { PRE(); vcmd = "cmd /c exit 0"; vmode = NULL; expect_fail(_popen(vcmd, vmode)); }
TC_CASE(wnull_cmd)  { PRE(); wcmd = NULL; wmode = L"r"; expect_fail(_wpopen(wcmd, wmode)); }
TC_CASE(wnull_mode) { PRE(); wcmd = L"cmd /c exit 0"; wmode = NULL; expect_fail(_wpopen(wcmd, wmode)); }
int main(void) { tc_begin(); TC_RUN(null_cmd); TC_RUN(null_mode); TC_RUN(wnull_cmd); TC_RUN(wnull_mode); return tc_end(); }
