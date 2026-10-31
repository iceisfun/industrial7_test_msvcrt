/* TC
 * id:    exp_neg_probe_getenv_s_range_buffer
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: getenv_s _putenv_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs getenv_s (does not state buffer content on ERANGE)
 * note:  PROBES: (1) buffer[0] is set to an empty string when getenv_s returns ERANGE; (2) getenv_s(&req, NULL, 0, name) returns 0
 *        (docs example uses this call to query the size but do not state the return value).
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

TC_CASE(buffer_empty_string)
{
    char b[3]; size_t req = 0;
    CHECK_EQ_I(p_penv(VN, "hello"), 0);
    b[0] = 'x';
    CHECK_EQ_I(p_genv(&req, b, sizeof b, VN), ERANGE);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buffer_size0_returns_0)
{ size_t req = 0; char *volatile nb = NULL; CHECK_EQ_I(p_penv(VN, "hello"), 0); CHECK_EQ_I(p_genv(&req, nb, 0, VN), 0); }
int main(void)
{
    tc_begin();
    p_genv = (genv_t)tc_crt_proc("getenv_s"); p_penv = (penv_t)tc_crt_proc("_putenv_s");
    if (!p_genv || !p_penv || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }

    TC_RUN(buffer_empty_string); TC_RUN(null_buffer_size0_returns_0);
    p_penv(VN, "");
    return tc_end();
}
