/* TC
 * id:    cv_neg_strtol_null
 * area:  conv
 * funcs: strtol strtoul _strtoi64
 * procs: strtoll
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtol: NULL string is an invalid parameter -> EINVAL, returns 0
 * note:  strtoll resolved via procs (not in all mingw import libs). endptr value after NULL input is deliberately not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef long long (__cdecl *strtoll_t)(const char *, char **, int);
static strtoll_t p_strtoll;

#define NULLCASE(call) do { char *e = NULL; int n0 = tc_iph_count; errno = 0; \
    CHECK_EQ_I(call(NULL, &e, 10), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, n0 + 1); } while (0)

TC_CASE(strtol_null) { NULLCASE(strtol); }
TC_CASE(strtoul_null) { NULLCASE(strtoul); }
TC_CASE(i64_null) { NULLCASE(_strtoi64); }
TC_CASE(strtoll_null)
{
    char *e = NULL; int n0 = tc_iph_count;
    if (!p_strtoll) TC_SKIP("strtoll not exported");
    errno = 0;
    CHECK_EQ_I(p_strtoll(NULL, &e, 10), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, n0 + 1);
}
TC_CASE(null_endptr_too) { int n0 = tc_iph_count; errno = 0; CHECK_EQ_I(strtol(NULL, NULL, 10), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, n0 + 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 2; }
    p_strtoll = (strtoll_t)tc_crt_proc("strtoll");
    TC_RUN(strtol_null); TC_RUN(strtoul_null); TC_RUN(i64_null); TC_RUN(strtoll_null); TC_RUN(null_endptr_too);
    return tc_end();
}
