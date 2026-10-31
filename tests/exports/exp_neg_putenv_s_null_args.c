/* TC
 * id:    exp_neg_putenv_s_null_args
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _putenv_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _putenv_s
 * note:  NULL args -> EINVAL
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *genv_t)(size_t *, char *, size_t, const char *);
typedef int (__cdecl *penv_t)(const char *, const char *);
static genv_t p_genv; static penv_t p_penv;
#define VN "TC_EXP_GETENV_VAR"

TC_CASE(null_name_einval)
{ const char *volatile n = NULL; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_penv(n, "v"), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_value_einval)
{ const char *volatile v = NULL; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_penv("TC_EXP_PUTENV_VAR", v), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    p_penv = (penv_t)tc_crt_proc("_putenv_s");
    if (!p_penv || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(null_name_einval); TC_RUN(null_value_einval);
    return tc_end();
}
