/* TC
 * id:    env_neg_wputenv_noequals_probe
 * area:  env
 * kind:  negative
 * funcs: _putenv _wputenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putenv ("envstring must be name=value"); behaviour for malformed strings is not specified
 * note:  Probes whether _putenv/_wputenv reject a string without '=', an empty string, and a string with an empty
 *        name ("=value"). Expected most likely: return -1 and the variable is not created.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int have_iph;

TC_CASE(narrow_no_equals)
{
    tc_iph_count = 0;
    CHECK_EQ_I(_putenv("TC_NEQ_A"), -1);
    CHECK(getenv("TC_NEQ_A") == NULL);
}
TC_CASE(wide_no_equals)
{
    CHECK_EQ_I(_wputenv(L"TC_NEQ_B"), -1);
    CHECK(_wgetenv(L"TC_NEQ_B") == NULL);
}
TC_CASE(empty_strings)
{
    CHECK_EQ_I(_putenv(""), -1);
    CHECK_EQ_I(_wputenv(L""), -1);
}
TC_CASE(empty_name)
{
    CHECK_EQ_I(_putenv("=tcvalue"), -1);
    CHECK_EQ_I(_wputenv(L"=tcvalue"), -1);
}
int main(void)
{
    tc_begin();
    have_iph = tc_iph_install(); (void)have_iph;
    TC_RUN(narrow_no_equals); TC_RUN(wide_no_equals); TC_RUN(empty_strings); TC_RUN(empty_name);
    return tc_end();
}
