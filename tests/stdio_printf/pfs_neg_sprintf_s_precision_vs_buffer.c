/* TC
 * id:    pfs_neg_sprintf_s_precision_vs_buffer
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s _snprintf_s
 * crt:   >=80 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s (overflow -> buffer[0]=0, handler, -1); _snprintf_s with _TRUNCATE (truncate, return -1, no handler); C11 7.21.6.1 precision of %s and negative * precision
 * note:  %.Ns limits the characters formatted, so the buffer needs N+1 bytes; a negative * precision is taken as omitted (C11 7.21.6.1p4).
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
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#ifdef _UCRT
# define P__SNPRINTF_S _snprintf_s
#else
# define P__SNPRINTF_S tc_p__snprintf_s
#endif
#include <stddef.h>


static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }
static const char *volatile f_p3 = "%.3s";
static const char *volatile f_p0 = "%.0s";
static const char *volatile f_pstar = "%.*s";
static const char *volatile s_abcdef = "abcdef";
static volatile int neg1 = -1;
static volatile int ten = 10;

TC_CASE(p3_fits_size4)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 4, f_p3, s_abcdef);
    CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(p3_fails_size3)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 3, f_p3, s_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(p0_fits_size1)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 1, f_p0, s_abcdef);
    CHECK_EQ_I(n, 0); CHECK(b[0] == 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(negative_star_precision)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 4, f_pstar, neg1, s_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(truncate_star_precision)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P__SNPRINTF_S(b, 4, (size_t)-1, f_pstar, ten, s_abcdef);
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
#ifndef _UCRT
    TC_LOAD_PROC(sprintf_s);
    TC_LOAD_PROC(_snprintf_s);
#endif
#ifndef _UCRT
    CHECK(P_SPRINTF_S != NULL);
    CHECK(P__SNPRINTF_S != NULL);
#endif
    TC_RUN(p3_fits_size4); TC_RUN(p3_fails_size3); TC_RUN(p0_fits_size1);
    TC_RUN(negative_star_precision); TC_RUN(truncate_star_precision);
    return tc_end();
}
