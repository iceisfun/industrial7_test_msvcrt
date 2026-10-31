/* TC
 * id:    exp_acrt_iob_func_ucrt
 * area:  exports
 * funcs: __acrt_iob_func
 * procs: _fileno
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   UCRT stdio.h: stdin == __acrt_iob_func(0)
 * note:  Do not test index>=3 (undefined).
 */
#include "tc.h"

#include <stdio.h>

TC_DECL_PROC(int, _fileno, (void *));

TC_CASE(nonnull_all_three) { CHECK(__acrt_iob_func(0) != NULL); CHECK(__acrt_iob_func(1) != NULL); CHECK(__acrt_iob_func(2) != NULL); }
TC_CASE(distinct)
{
    FILE *a = __acrt_iob_func(0), *b = __acrt_iob_func(1), *c = __acrt_iob_func(2);
    CHECK(a != b); CHECK(b != c); CHECK(a != c);
}
TC_CASE(fileno_0_1_2)
{
    int i;
    if (!tc_p__fileno) TC_SKIP("_fileno missing");
    for (i = 0; i < 3; i++) CHECK_EQ_I(tc_p__fileno(__acrt_iob_func(i)), i);
}
TC_CASE(stable_across_calls)
{
    int i;
    for (i = 0; i < 3; i++) CHECK_EQ_P(__acrt_iob_func(i), __acrt_iob_func(i));
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_fileno);
    TC_RUN(nonnull_all_three); TC_RUN(distinct); TC_RUN(fileno_0_1_2); TC_RUN(stable_across_calls);
    return tc_end();
}
