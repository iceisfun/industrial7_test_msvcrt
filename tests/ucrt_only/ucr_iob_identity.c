/* TC
 * id:    ucr_iob_identity
 * area:  ucrt_only
 * funcs: __acrt_iob_func _fileno
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   UCRT stdio.h: stdin/stdout/stderr expand to __acrt_iob_func(0/1/2); MS docs _fileno, stdin/stdout/stderr
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned v0 = 0, v1 = 1, v2 = 2;

TC_CASE(non_null) { CHECK(__acrt_iob_func(v0) != NULL); CHECK(__acrt_iob_func(v1) != NULL); CHECK(__acrt_iob_func(v2) != NULL); }
TC_CASE(distinct)
{
    FILE *a = __acrt_iob_func(v0), *b = __acrt_iob_func(v1), *c = __acrt_iob_func(v2);
    CHECK(a != b); CHECK(b != c); CHECK(a != c);
}
TC_CASE(stable) { CHECK_EQ_P(__acrt_iob_func(v1), __acrt_iob_func(v1)); CHECK_EQ_P(__acrt_iob_func(v0), __acrt_iob_func(v0)); CHECK_EQ_P(__acrt_iob_func(v2), __acrt_iob_func(v2)); }
TC_CASE(macros) { CHECK_EQ_P(stdin, __acrt_iob_func(v0)); CHECK_EQ_P(stdout, __acrt_iob_func(v1)); CHECK_EQ_P(stderr, __acrt_iob_func(v2)); }
TC_CASE(fileno) { CHECK_EQ_I(_fileno(__acrt_iob_func(v0)), 0); CHECK_EQ_I(_fileno(__acrt_iob_func(v1)), 1); CHECK_EQ_I(_fileno(__acrt_iob_func(v2)), 2); }

int main(void) { tc_begin(); TC_RUN(non_null); TC_RUN(distinct); TC_RUN(stable); TC_RUN(macros); TC_RUN(fileno); return tc_end(); }
