/* TC
 * id:    str_neg_strtok_s_invalid
 * area:  string
 * funcs:
 * procs: strtok_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s: context NULL or delimiters NULL -> EINVAL, invalid parameter handler invoked
 */
#include "tc.h"
#include <errno.h>

typedef char *(__cdecl *strtok_s_t)(char *, const char *, char **);
static strtok_s_t p_strtok_s;

TC_CASE(context_null)
{
    char b[] = "a b"; int before;
    if (!p_strtok_s) TC_SKIP("strtok_s not exported");
    errno = 0; before = tc_iph_count;
    CHECK(p_strtok_s(b, " ", NULL) == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - before, 1);
    CHECK_STR(b, "a b");
}
TC_CASE(delims_null)
{
    char b[] = "a b"; char *ctx = NULL; int before;
    if (!p_strtok_s) TC_SKIP("strtok_s not exported");
    errno = 0; before = tc_iph_count;
    CHECK(p_strtok_s(b, NULL, &ctx) == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - before, 1);
}
int main(void)
{
    tc_begin();
    p_strtok_s = (strtok_s_t)tc_crt_proc("strtok_s");
    CHECK(tc_iph_install());
    TC_RUN(context_null); TC_RUN(delims_null);
    return tc_end();
}
