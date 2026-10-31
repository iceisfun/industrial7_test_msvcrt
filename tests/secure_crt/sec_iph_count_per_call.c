/* TC
 * id:    sec_iph_count_per_call
 * area:  secure_crt
 * funcs: strcpy_s memcpy_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter Validation; strcpy_s; memcpy_s
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static char *volatile v_nulld;
TC_CASE(success_does_not_call)
{
    char d[8]; tc_iph_count = 0;
    CHECK_EQ_I(strcpy_s(d, sizeof d, "abc"), 0);
    CHECK_EQ_I(memcpy_s(d, sizeof d, "xy", 2), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(one_failure_one_call)
{
    char d[8]; tc_iph_count = 0;
    CHECK_EQ_I(strcpy_s(v_nulld, 8, "abc"), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(two_failures_two_calls)
{
    char d[8]; tc_iph_count = 0;
    strcpy_s(v_nulld, 8, "abc");
    memcpy_s(NULL, 4, "abc", 3);
    CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(handler_returns_and_function_returns_errcode)
{
    char d[2]; int r; tc_iph_count = 0;
    r = strcpy_s(d, sizeof d, "toolong");
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    r = memcpy_s(d, 1, "ab", 2);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(process_continues)
{
    char d[4]; volatile int reached = 0;
    strcpy_s(v_nulld, 4, "a");
    reached = 1;
    CHECK(reached);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(success_does_not_call); TC_RUN(one_failure_one_call); TC_RUN(two_failures_two_calls);
    TC_RUN(handler_returns_and_function_returns_errcode); TC_RUN(process_continues);
    return tc_end();
}
