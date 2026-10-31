/* TC
 * id:    str_neg_stricmp_count_above_intmax_probe
 * area:  string
 * funcs: _strnicmp
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strnicmp (returns _NLSCMPERROR = INT_MAX on error)
 * note:  PROBE: ucrt _strnicmp validates count <= INT_MAX; with count=(size_t)-1 expect invalid-parameter handler once, EINVAL, return INT_MAX (_NLSCMPERROR).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(huge_count)
{
    volatile const char *a = "abc", *b = "ABC"; volatile size_t n = (size_t)-1; int r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _strnicmp((const char *)a, (const char *)b, n);
    CHECK_EQ_I(r, 0x7fffffff);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(huge_count); return tc_end(); }
