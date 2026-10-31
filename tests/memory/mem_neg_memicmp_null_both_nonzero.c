/* TC
 * id:    mem_neg_memicmp_null_both_nonzero
 * area:  memory
 * kind:  negative
 * funcs: _memicmp
 * procs: _memicmp_l
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _memicmp (returns _NLSCMPERROR = INT_MAX on error), "Parameter validation"
 * note:  PROBE: with both buffers NULL and count>0, the result is INT_MAX (_NLSCMPERROR) with one handler call and errno EINVAL;
 *        parameter validation precedes any read even for count == SIZE_MAX.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>

typedef int (__cdecl *mil_t)(const void *, const void *, size_t, void *);
static mil_t p_mil;
static const void *volatile v_null = NULL;
static volatile size_t v_four = 4, v_max = (size_t)-1;

TC_CASE(memicmp_both_null)
{
    int r; tc_iph_count = 0; errno = 0;
    r = _memicmp(v_null, v_null, v_four);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(memicmp_l_both_null)
{
    int r; CHECK(p_mil != NULL); if (!p_mil) return;
    tc_iph_count = 0; errno = 0;
    r = p_mil(v_null, v_null, v_four, NULL);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(huge_count_null_first)
{
    static char buf[8] = "abcdefg"; int r;
    tc_iph_count = 0; errno = 0;
    r = _memicmp(v_null, buf, v_max);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    p_mil = (mil_t)tc_crt_proc("_memicmp_l");
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup no handler API\n"); return 1; }
    TC_RUN(memicmp_both_null); TC_RUN(memicmp_l_both_null); TC_RUN(huge_count_null_first);
    return tc_end();
}
