/* TC
 * id:    str_neg_stricmp_null
 * area:  string
 * funcs: _stricmp _strnicmp
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _stricmp (return _NLSCMPERROR = INT_MAX on error), parameter validation
 * note:  ucrtbase only: older CRTs may skip validation on the C-locale fast path; msvcr80-120 are covered by str_neg_stricmp_null_win7_probe.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>

static volatile const char *pa, *pb;
#define NLSERR INT_MAX

TC_CASE(first_null)
{
    int r; tc_iph_count = 0; errno = 0; pa = NULL; pb = "a";
    r = _stricmp((const char *)pa, (const char *)pb);
    CHECK_EQ_I(r, NLSERR); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(second_null)
{
    int r; tc_iph_count = 0; errno = 0; pa = "a"; pb = NULL;
    r = _stricmp((const char *)pa, (const char *)pb);
    CHECK_EQ_I(r, NLSERR); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(strnicmp_null)
{
    int r; tc_iph_count = 0; errno = 0; pa = NULL; pb = NULL;
    r = _strnicmp((const char *)pa, (const char *)pb, 3);
    CHECK_EQ_I(r, NLSERR); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; pa = NULL; pb = "a";
    r = _strnicmp((const char *)pa, (const char *)pb, 3);
    CHECK_EQ_I(r, NLSERR); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; pa = "a"; pb = NULL;
    r = _strnicmp((const char *)pa, (const char *)pb, 3);
    CHECK_EQ_I(r, NLSERR); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(first_null); TC_RUN(second_null); TC_RUN(strnicmp_null);
    return tc_end();
}
