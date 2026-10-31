/* TC
 * id:    err_iph_noinfo_calls_handler
 * area:  errno_param
 * funcs: _invalid_parameter_noinfo _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _invalid_parameter_noinfo (calls the invalid parameter handler without information)
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(calls_handler_once)
{
    tc_iph_count = 0;
    _invalid_parameter_noinfo();
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(accumulates)
{
    int i; tc_iph_count = 0;
    for (i = 0; i < 3; i++) _invalid_parameter_noinfo();
    CHECK_EQ_I(tc_iph_count, 3);
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(tc_iph);
    TC_RUN(calls_handler_once); TC_RUN(accumulates);
    return tc_end();
}
