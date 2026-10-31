/* TC
 * id:    err_neg_iph_exactly_once_per_call
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s strcat_s memcpy_s wcscpy_s _itoa_s sprintf_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter Validation"; docs of each function (error conditions invoke the invalid parameter handler)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <io.h>
#include <errno.h>

static char * volatile v_null = NULL;
static volatile int v_bad_fd = -1;
static volatile size_t v_one = 1;

TC_CASE(strcpy_s_null) { tc_iph_count = 0; (void)(strcpy_s(v_null, 10, "a")); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strcat_s_small) { tc_iph_count = 0; (void)((void)strcat_s((char[]){ 'a', 0 }, v_one, "b")); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(memcpy_s_null) { tc_iph_count = 0; (void)(memcpy_s(v_null, 10, "ab", 2)); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcscpy_s_small) { tc_iph_count = 0; (void)(wcscpy_s((wchar_t[2]){ 0 }, v_one, L"abc")); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(itoa_s_small) { tc_iph_count = 0; (void)(_itoa_s(12345, (char[2]){ 0 }, v_one + 1, 10)); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(sprintf_s_null) { tc_iph_count = 0; (void)(sprintf_s(v_null, 10, "x")); CHECK_EQ_I(tc_iph_count, 1); }

TC_CASE(valid_no_calls)
{
    char d[16]; wchar_t w[8]; int r = 0;
    tc_iph_count = 0;
    r |= strcpy_s(d, sizeof d, "ab"); r |= strcat_s(d, sizeof d, "cd"); r |= memcpy_s(d, sizeof d, "xy", 2);
    r |= wcscpy_s(w, 8, L"abc"); r |= _itoa_s(123, d, sizeof d, 10); r |= sprintf_s(d, sizeof d, "%d", 5) < 0;
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no handler api\n"); return 1; }
    TC_RUN(strcpy_s_null); TC_RUN(strcat_s_small); TC_RUN(memcpy_s_null); TC_RUN(wcscpy_s_small);
    TC_RUN(itoa_s_small); TC_RUN(sprintf_s_null); TC_RUN(valid_no_calls);
    return tc_end();
}
