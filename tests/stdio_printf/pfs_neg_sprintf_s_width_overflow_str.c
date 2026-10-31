/* TC
 * id:    pfs_neg_sprintf_s_width_overflow_str
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s swprintf_s
 * crt:   >=80 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s / swprintf_s: if the formatted output (including NUL) does not fit, buffer[0] = 0 and the invalid parameter handler is invoked, return -1
 * note:  Width padding alone makes the output too long; the string argument itself is short.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#ifdef _UCRT
# define P_SPRINTF_S sprintf_s
#else
# define P_SPRINTF_S tc_p_sprintf_s
#endif
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
#ifdef _UCRT
# define P_SWPRINTF_S swprintf_s
#else
# define P_SWPRINTF_S tc_p_swprintf_s
#endif


static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }
static const char *volatile f_20s = "%20s";
static const char *volatile f_m20s = "%-20s";
static const char *volatile f_starS = "%*s";
static const char *volatile f_8s = "%8s";
static const char *volatile f_7s = "%7s";
static const wchar_t *volatile wf_20s = L"%20s";
static volatile int w20 = 20;

TC_CASE(padding_overflow)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 8, f_20s, "ab");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(left_and_star_variants)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 8, f_m20s, "ab");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
    fill(b);
    n = P_SPRINTF_S(b, 8, f_starS, w20, "ab");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 2); CHECK(b[0] == 0);
}
TC_CASE(boundary)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 8, f_8s, "abcdefgh");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
    fill(b);
    n = P_SPRINTF_S(b, 8, f_7s, "ab");
    CHECK_EQ_I(n, 7); CHECK_STR(b, "     ab"); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide)
{
    wchar_t w[16]; int n, i; for (i = 0; i < 16; i++) w[i] = L'Z'; tc_iph_count = 0;
    n = P_SWPRINTF_S(w, 8, wf_20s, L"ab");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(w[0] == 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
#ifndef _UCRT
    TC_LOAD_PROC(sprintf_s);
    TC_LOAD_PROC(swprintf_s);
#endif
#ifndef _UCRT
    CHECK(P_SPRINTF_S != NULL);
    CHECK(P_SWPRINTF_S != NULL);
#endif
    TC_RUN(padding_overflow); TC_RUN(left_and_star_variants); TC_RUN(boundary); TC_RUN(wide);
    return tc_end();
}
