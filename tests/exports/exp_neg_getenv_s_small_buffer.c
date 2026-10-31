/* TC
 * id:    exp_neg_getenv_s_small_buffer
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: getenv_s _putenv_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs getenv_s
 * note:  ERANGE with required size incl NUL; NULL buffer + size 0 queries size (return value of that call is probed separately).
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

TC_CASE(erange_and_required_size)
{
    char b[3]; size_t req = 0; int r;
    CHECK_EQ_I(p_penv(VN, "hello"), 0);
    errno = 0;
    r = p_genv(&req, b, sizeof b, VN);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_U(req, 6);
}
TC_CASE(exact_size_ok)
{
    char b[6]; size_t req = 0;
    CHECK_EQ_I(p_penv(VN, "hello"), 0);
    CHECK_EQ_I(p_genv(&req, b, sizeof b, VN), 0);
    CHECK_EQ_U(req, 6);
    CHECK_STR(b, "hello");
}
TC_CASE(query_size_with_null_buffer_ok)
{
    size_t req = 0; char *volatile nb = NULL;
    CHECK_EQ_I(p_penv(VN, "hello"), 0);
    tc_iph_count = 0;
    (void)p_genv(&req, nb, 0, VN);
    CHECK_EQ_U(req, 6);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_genv = (genv_t)tc_crt_proc("getenv_s"); p_penv = (penv_t)tc_crt_proc("_putenv_s");
    if (!p_genv || !p_penv || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }

    TC_RUN(erange_and_required_size); TC_RUN(exact_size_ok); TC_RUN(query_size_with_null_buffer_ok);
    p_penv(VN, "");
    return tc_end();
}
