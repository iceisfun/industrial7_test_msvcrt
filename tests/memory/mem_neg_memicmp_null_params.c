/* TC
 * id:    mem_neg_memicmp_null_params
 * area:  memory
 * kind:  negative
 * funcs: _memicmp
 * crt:   >=80
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs "Parameter validation" (secure CRT functions call the invalid parameter handler, errno EINVAL); _memicmp validating NULL buffers is not spelled out on its page
 * note:  Return value on failure is covered by mem_neg_memicmp_null_retval_probe.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static char buf[8] = "abcdefg";
static void *volatile v_null = NULL;
static volatile size_t v_n = 4;

TC_CASE(first_null_handler_called)
{
    tc_iph_count = 0; (void)_memicmp(v_null, buf, v_n);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(second_null_handler_called)
{
    tc_iph_count = 0; (void)_memicmp(buf, v_null, v_n);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(errno_EINVAL)
{
    errno = 0; (void)_memicmp(v_null, buf, v_n);
    CHECK_EQ_I(errno, EINVAL);
    errno = 0; (void)_memicmp(buf, v_null, v_n);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(both_null_handler_called)
{
    tc_iph_count = 0; (void)_memicmp(v_null, v_null, v_n);
    CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup no handler API\n"); return 1; }
    TC_RUN(first_null_handler_called); TC_RUN(second_null_handler_called);
    TC_RUN(errno_EINVAL); TC_RUN(both_null_handler_called);
    return tc_end();
}
