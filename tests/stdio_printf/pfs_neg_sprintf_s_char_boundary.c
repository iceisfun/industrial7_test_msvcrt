/* TC
 * id:    pfs_neg_sprintf_s_char_boundary
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s swprintf_s
 * crt:   >=80 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s / swprintf_s: the terminating NUL counts toward sizeInBytes; on overflow buffer[0]=0, handler invoked, -1
 * note:  %c and %5c exact boundaries: output length must be strictly less than the buffer size.
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
static void wfill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const char *volatile f_c = "%c";
static const char *volatile f_5c = "%5c";
static const char *volatile f_lc = "%lc";
static const wchar_t *volatile wf_c = L"%c";
static const wchar_t *volatile wf_hc = L"%hc";
static const wchar_t *volatile wf_lc = L"%lc";
static volatile int cx = 'x';
static volatile wchar_t wcx = L'x';

TC_CASE(c_size1_fails)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 1, f_c, cx);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(c_size2_ok)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 2, f_c, cx);
    CHECK_EQ_I(n, 1); CHECK_STR(b, "x"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(width5)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 5, f_5c, cx);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
    fill(b);
    n = P_SPRINTF_S(b, 6, f_5c, cx);
    CHECK_EQ_I(n, 5); CHECK_STR(b, "    x"); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(lc_narrow_buffer)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = P_SPRINTF_S(b, 1, f_lc, (wint_t)wcx);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
    fill(b);
    n = P_SPRINTF_S(b, 2, f_lc, (wint_t)wcx);
    CHECK_EQ_I(n, 1); CHECK_STR(b, "x");
}
TC_CASE(wide_buffer)
{
    wchar_t w[16]; int n; wfill(w); tc_iph_count = 0;
    n = P_SWPRINTF_S(w, 1, wf_c, (wint_t)wcx);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(w[0] == 0);
    wfill(w);
    n = P_SWPRINTF_S(w, 2, wf_c, (wint_t)wcx);
    CHECK_EQ_I(n, 1); CHECK(w[0] == L'x' && w[1] == 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_buffer_lc_hc)
{
    wchar_t w[16]; int n; wfill(w); tc_iph_count = 0;
    n = P_SWPRINTF_S(w, 1, wf_lc, (wint_t)wcx);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(w[0] == 0);
    wfill(w);
    n = P_SWPRINTF_S(w, 1, wf_hc, cx);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 2); CHECK(w[0] == 0);
    wfill(w);
    n = P_SWPRINTF_S(w, 2, wf_hc, cx);
    CHECK_EQ_I(n, 1); CHECK(w[0] == L'x' && w[1] == 0);
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
    TC_RUN(c_size1_fails); TC_RUN(c_size2_ok); TC_RUN(width5); TC_RUN(lc_narrow_buffer);
    TC_RUN(wide_buffer); TC_RUN(wide_buffer_lc_hc);
    return tc_end();
}
