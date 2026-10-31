/* TC
 * id:    mem_neg_memicmp_zero_count_null_probe
 * area:  memory
 * kind:  negative
 * funcs: _memicmp
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _memicmp (validation is stated for NULL buffers; count == 0 interplay not documented)
 * note:  PROBES _memicmp(NULL, NULL, 0): expected to return 0 without invoking the invalid-parameter handler
 *        (the validation is believed to be "count == 0 || (buf1 && buf2)").
 */
#include "tc.h"
#include <string.h>

static void *volatile v_null = NULL;
static volatile size_t v_zero = 0;

TC_CASE(returns_0) { CHECK_EQ_I(_memicmp(v_null, v_null, v_zero), 0); }
TC_CASE(handler_not_called) { tc_iph_count = 0; (void)_memicmp(v_null, v_null, v_zero); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup no handler API\n"); return 1; }
    TC_RUN(returns_0); TC_RUN(handler_not_called);
    return tc_end();
}
