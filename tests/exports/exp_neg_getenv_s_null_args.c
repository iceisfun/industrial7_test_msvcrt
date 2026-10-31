/* TC
 * id:    exp_neg_getenv_s_null_args
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: getenv_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs getenv_s
 * note:  invalid argument table
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

TC_CASE(null_preturn_einval)
{ char b[8]; size_t *volatile pr = NULL; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_genv(pr, b, sizeof b, "PATH"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buffer_nonzero_size_einval)
{ size_t req = 0; char *volatile nb = NULL; tc_iph_count = 0; CHECK_EQ_I(p_genv(&req, nb, 8, "PATH"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_varname_einval)
{ char b[8]; size_t req = 0; const char *volatile vn = NULL; tc_iph_count = 0; CHECK_EQ_I(p_genv(&req, b, sizeof b, vn), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    p_genv = (genv_t)tc_crt_proc("getenv_s");
    if (!p_genv || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(null_preturn_einval); TC_RUN(null_buffer_nonzero_size_einval); TC_RUN(null_varname_einval);
    return tc_end();
}
