/* TC
 * id:    dat_iob_identities
 * area:  data_exports
 * funcs: __acrt_iob_func _fileno
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fileno, stdin/stdout/stderr (file descriptors 0, 1, 2)
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned v_i;

TC_CASE(three_distinct_nonnull)
{
    FILE *a = __acrt_iob_func(0), *b = __acrt_iob_func(1), *c = __acrt_iob_func(2);
    CHECK(a && b && c); CHECK(a != b && b != c && a != c);
}
TC_CASE(stable_across_calls) { v_i = 1; CHECK_EQ_P(__acrt_iob_func(v_i), __acrt_iob_func(v_i)); v_i = 2; CHECK_EQ_P(__acrt_iob_func(v_i), stderr); }
TC_CASE(fileno_0_1_2) { CHECK_EQ_I(_fileno(__acrt_iob_func(0)), 0); CHECK_EQ_I(_fileno(__acrt_iob_func(1)), 1); CHECK_EQ_I(_fileno(__acrt_iob_func(2)), 2); }
TC_CASE(stdin_macro_equals_iob_func0) { v_i = 0; CHECK_EQ_P(stdin, __acrt_iob_func(v_i)); }
TC_CASE(stdout_stderr_macros_match) { CHECK_EQ_P(stdout, __acrt_iob_func(1)); CHECK_EQ_P(stderr, __acrt_iob_func(2)); CHECK_EQ_I(_fileno(stdout), 1); CHECK_EQ_I(_fileno(stderr), 2); }

int main(void)
{
    tc_begin();
    TC_RUN(three_distinct_nonnull); TC_RUN(stable_across_calls); TC_RUN(fileno_0_1_2);
    TC_RUN(stdin_macro_equals_iob_func0); TC_RUN(stdout_stderr_macros_match);
    return tc_end();
}
