/* TC
 * id:    err_memcpy_s_zero_count_null_probe
 * area:  errno_param
 * funcs: memcpy_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs memcpy_s
 * note:  PROBE: memcpy_s with count == 0 returns 0 immediately, even with NULL dest/src, without calling the
 *        invalid-parameter handler (the docs error table does not mention count == 0).
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static void * volatile v_null = NULL;
static volatile size_t v_n;

TC_CASE(zero_count_null_ok)
{
    const void * volatile s = NULL; int r; tc_iph_count = 0; v_n = 0;
    r = memcpy_s(v_null, 0, s, v_n);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(tc_iph);
    TC_RUN(zero_count_null_ok);
    return tc_end();
}
