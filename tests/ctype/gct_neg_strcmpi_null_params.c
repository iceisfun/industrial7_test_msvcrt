/* TC
 * id:    gct_neg_strcmpi_null_params
 * area:  ctype
 * funcs: _stricmp _stricoll _memicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _stricmp, _wcsicmp, _mbsicmp; _stricoll; _memicmp ("_NLSCMPERROR, errno EINVAL" on invalid parameter)
 * note:  _strcmpi is not exported by every CRT (inline alias), so _stricmp is tested. NULL _memicmp with count 0 is covered by gct_neg_memicmp_zero_count_probe.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>

static const char * volatile nul = NULL;
static const char * volatile a = "a";

#define EXPECT_ERR(call) do { int r; int b = tc_iph_count; errno = 0; r = (call); \
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, b + 1); } while (0)

TC_CASE(stricmp_first_null)  { EXPECT_ERR(_stricmp(nul, a)); }
TC_CASE(stricmp_second_null) { EXPECT_ERR(_stricmp(a, nul)); }
TC_CASE(stricoll_first_null) { EXPECT_ERR(_stricoll(nul, a)); }
TC_CASE(stricoll_second_null){ EXPECT_ERR(_stricoll(a, nul)); }
TC_CASE(memicmp_first_null)  { EXPECT_ERR(_memicmp(nul, a, 1)); }
TC_CASE(memicmp_second_null) { EXPECT_ERR(_memicmp(a, nul, 1)); }
TC_CASE(valid_after_failure)
{
    CHECK_EQ_I(_stricmp(a, "A"), 0);
    CHECK_EQ_I(_stricoll(a, "A"), 0);
    CHECK_EQ_I(_memicmp(a, "A", 1), 0);
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(stricmp_first_null); TC_RUN(stricmp_second_null); TC_RUN(stricoll_first_null); TC_RUN(stricoll_second_null);
    TC_RUN(memicmp_first_null); TC_RUN(memicmp_second_null); TC_RUN(valid_after_failure);
    return tc_end();
}
