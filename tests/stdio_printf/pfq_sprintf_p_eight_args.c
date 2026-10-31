/* TC
 * id:    pfq_sprintf_p_eight_args
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters (reordering, reuse, up to _ARGMAX positions)
 */
#include "tc.h"
#include "pfq_shim.h"

TC_CASE(reversed_eight)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "%8$d%7$d%6$d%5$d%4$d%3$d%2$d%1$d", 1, 2, 3, 4, 5, 6, 7, 8);
    CHECK_EQ_I(r, 8); CHECK_STR(b, "87654321");
}
TC_CASE(mixed_types_eight)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "%4$s|%3$c|%2$x|%1$d|%5$.1f|%6$s|%7$u|%8$o", 1, 255, 'q', "s4", 2.5, "s6", 7u, 8);
    CHECK_EQ_I(r, 21); CHECK_STR(b, "s4|q|ff|1|2.5|s6|7|10");
}
TC_CASE(sparse_reuse)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "%3$d%1$d%3$d%2$d", 1, 2, 3);
    CHECK_EQ_I(r, 4); CHECK_STR(b, "3132");
}
int main(void)
{
    tc_begin();
    TC_RUN(reversed_eight); TC_RUN(mixed_types_eight); TC_RUN(sparse_reuse);
    return tc_end();
}
