/* TC
 * id:    exp_neg_getenv_s_null_args
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: getenv_s
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs getenv_s
 * note:  NULL pReturnValue and NULL buffer with size>0 -> EINVAL + one handler call (matches the MS docs table). NULL varname: Win10 22H2 ucrtbase does not
 *        treat it as an error (observed return 0, required size 0, empty buffer, no handler call).
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
TC_CASE(null_varname_not_validated)
{ char b[8]; size_t req = 9; const char *volatile vn = NULL; b[0] = 'x'; tc_iph_count = 0;
  /* observed on Win10 22H2: a NULL varname is not rejected: returns 0, *required = 0, buffer[0] = 0, no handler call */
  CHECK_EQ_I(p_genv(&req, b, sizeof b, vn), 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_U(req, 0); CHECK_EQ_I(b[0], 0); }
int main(void)
{
    tc_begin();
    p_genv = (genv_t)tc_crt_proc("getenv_s");
    if (!p_genv || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(null_preturn_einval); TC_RUN(null_buffer_nonzero_size_einval); TC_RUN(null_varname_not_validated);
    return tc_end();
}
