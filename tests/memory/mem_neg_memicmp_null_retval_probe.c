/* TC
 * id:    mem_neg_memicmp_null_retval_probe
 * area:  memory
 * kind:  negative
 * funcs: _memicmp
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _stricmp family return _NLSCMPERROR (INT_MAX) on invalid parameters
 * note:  PROBES whether _memicmp(NULL, buf, 4) / _memicmp(buf, NULL, 4) return _NLSCMPERROR (0x7fffffff) after the
 *        invalid-parameter handler returns. Handler is installed so the call returns.
 */
#include "tc.h"
#include <string.h>

static char buf[8] = "abcdefg";
static void *volatile v_null = NULL;
static volatile size_t v_n = 4;

TC_CASE(first_null_returns_INT_MAX) { CHECK_EQ_I(_memicmp(v_null, buf, v_n), 0x7fffffff); }
TC_CASE(second_null_returns_INT_MAX) { CHECK_EQ_I(_memicmp(buf, v_null, v_n), 0x7fffffff); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup no handler API\n"); return 1; }
    TC_RUN(first_null_returns_INT_MAX); TC_RUN(second_null_returns_INT_MAX);
    return tc_end();
}
