/* TC
 * id:    pfp_n_enabled_sizes
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf width specification / size prefixes: h, l, ll, I64 (ll, z, j, t, hh in UCRT); type n
 * note:  %I64n is asserted on ucrtbase only: whether the pre-UCRT CRTs store 64 bits for %I64n is probed separately (pfp_n_i64_old_probe).
 *        Each case pre-fills a 16-byte buffer with 0xCC and checks exactly which bytes %n stores. ll/z/j/t cases are ucrtbase only.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
static union { long long align; unsigned char c[16]; } u;
static void fill(void) { memset(u.c, 0xCC, sizeof u.c); }

TC_CASE(hn)
{
    char b[16]; fill();
    sprintf(b, "abc%hn", (short *)u.c);
    CHECK_EQ_U(u.c[0], 3); CHECK_EQ_U(u.c[1], 0); CHECK_EQ_U(u.c[2], 0xCC); CHECK_EQ_U(u.c[3], 0xCC);
}
TC_CASE(ln)
{
    char b[16]; fill();
    sprintf(b, "abcd%ln", (long *)u.c);
    CHECK_EQ_U(u.c[0], 4); CHECK_EQ_U(u.c[1], 0); CHECK_EQ_U(u.c[2], 0); CHECK_EQ_U(u.c[3], 0); CHECK_EQ_U(u.c[4], 0xCC);
}
TC_CASE(plain_n)
{
    char b[16]; fill();
    sprintf(b, "abcde%n", (int *)u.c);
    CHECK_EQ_U(u.c[0], 5); CHECK_EQ_U(u.c[3], 0); CHECK_EQ_U(u.c[4], 0xCC);
}
TC_CASE(i64n)
{
#if TC_CRT_VER >= 140
    char b[16]; fill();
    sprintf(b, "abc%I64n", (long long *)u.c);
    CHECK_EQ_U(u.c[0], 3); CHECK_EQ_U(u.c[1], 0); CHECK_EQ_U(u.c[7], 0); CHECK_EQ_U(u.c[8], 0xCC);
#else
    TC_SKIP("ucrt only; see pfp_n_i64_old_probe");
#endif
}
TC_CASE(ucrt_ll_z_j_t)
{
#if TC_CRT_VER >= 140
    char b[16];
    fill(); sprintf(b, "abc%lln", (long long *)u.c);
    CHECK_EQ_U(u.c[0], 3); CHECK_EQ_U(u.c[7], 0); CHECK_EQ_U(u.c[8], 0xCC);
    fill(); sprintf(b, "abc%zn", (size_t *)u.c);
    CHECK_EQ_U(u.c[0], 3); CHECK_EQ_U(u.c[sizeof(size_t) - 1], 0); CHECK_EQ_U(u.c[sizeof(size_t)], 0xCC);
    fill(); sprintf(b, "abc%jn", (intmax_t *)u.c);
    CHECK_EQ_U(u.c[0], 3); CHECK_EQ_U(u.c[7], 0); CHECK_EQ_U(u.c[8], 0xCC);
    fill(); sprintf(b, "abc%tn", (ptrdiff_t *)u.c);
    CHECK_EQ_U(u.c[0], 3); CHECK_EQ_U(u.c[sizeof(ptrdiff_t) - 1], 0); CHECK_EQ_U(u.c[sizeof(ptrdiff_t)], 0xCC);
#else
    TC_SKIP("ucrt only");
#endif
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(hn); TC_RUN(ln); TC_RUN(plain_n); TC_RUN(i64n); TC_RUN(ucrt_ll_z_j_t);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
