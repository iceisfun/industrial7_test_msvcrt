/* TC
 * id:    pfp_neg_n_redisabled
 * area:  stdio_printf
 * funcs: sprintf _set_printf_count_output
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, _get_printf_count_output: %n disabled -> invalid parameter
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

typedef int (__cdecl *get_t)(void);
static get_t p_get;

TC_CASE(enable_then_disable)
{
    char b[32]; int n = 0x55; int r;
    _set_printf_count_output(1);
    r = sprintf(b, "ab%ncd", &n);
    CHECK_EQ_I(r, 4); CHECK_EQ_I(n, 2); CHECK_STR(b, "abcd");
    _set_printf_count_output(0);
    n = 0x55; b[0] = 'Z';
    tc_iph_count = 0;
    r = sprintf(b, "ab%ncd", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x55);
}
TC_CASE(get_after_disable)
{
    if (!p_get) TC_SKIP("_get_printf_count_output not exported");
    _set_printf_count_output(1);
    CHECK(p_get() != 0);
    _set_printf_count_output(0);
    CHECK_EQ_I(p_get(), 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    p_get = (get_t)tc_crt_proc("_get_printf_count_output");
    TC_RUN(enable_then_disable); TC_RUN(get_after_disable);
    _set_printf_count_output(0);
    return tc_end();
}
