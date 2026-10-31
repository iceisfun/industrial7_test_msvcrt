/* TC
 * id:    pfm_2_neg_conv_n_disabled_iph_ucrt
 * area:  stdio_printf
 * funcs: sprintf _snprintf_s fprintf
 * procs: _set_printf_count_output
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_printf_count_output: %n is disabled by default; printf type field n
 * note:  %n (also with h, l, ll size prefixes) fails with -1, EINVAL and one invalid-parameter call, and stores nothing.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(plain_n)   { char b[32]; int n = 0x5a5a5a5a, r; tc_iph_count = 0; errno = 0; r = sprintf(b, "ab%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x5a5a5a5a); }
TC_CASE(hn)        { char b[32]; short n = 0x5a5a; int r; tc_iph_count = 0; errno = 0; r = sprintf(b, "ab%hn", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x5a5a); }
TC_CASE(lln)       { char b[32]; long long n = 0x5a5a5a5a5a5a5a5aLL; int r; tc_iph_count = 0; errno = 0; r = sprintf(b, "ab%lln", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK(n == 0x5a5a5a5a5a5a5a5aLL); }
TC_CASE(snprintf_s){ char b[32]; int n = 0x5a5a5a5a, r; tc_iph_count = 0; errno = 0; r = _snprintf_s(b, sizeof b, _TRUNCATE, "ab%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x5a5a5a5a); }
TC_CASE(fprintf_nothing)
{
    const char *name = "tc_pfm_2_neg_conv_n_disabled_iph_ucrt_a.tmp"; FILE *f; int n = 7, r; long sz = -1;
    f = fopen(name, "w+b"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, "%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 7);
    fflush(f); fseek(f, 0, SEEK_END); sz = ftell(f); CHECK_EQ_I(sz, 0);
    fclose(f); remove(name);
}
TC_CASE(enabled_after)
{
    char b[32]; int n = 0x5a5a5a5a, r, old;
    old = tc_p__set_printf_count_output(1); CHECK_EQ_I(old, 0);
    tc_iph_count = 0; r = sprintf(b, "ab%n", &n);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(n, 2); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "ab");
    tc_p__set_printf_count_output(0);
    n = 0x5a5a5a5a; tc_iph_count = 0; r = sprintf(b, "ab%n", &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x5a5a5a5a);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    TC_RUN(plain_n); TC_RUN(hn); TC_RUN(lln); TC_RUN(snprintf_s); TC_RUN(fprintf_nothing); TC_RUN(enabled_after);
    return tc_end();
}
