/* TC
 * id:    pfr_neg_probe_swprintf_s_size_one
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s swprintf_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s, swprintf_s (buffer too small: ERANGE, buffer[0]=0, return -1)
 * note:  PROBE: boundary of NUL accounting with sizeOfBuffer==1. An empty output fits (return 0, b[0]==0);
 *        a one-character output does not fit: -1, ERANGE, b[0]==0.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#if TC_CRT_VER < 140
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
#define sprintf_s tc_p_sprintf_s
#define swprintf_s tc_p_swprintf_s
#endif
static const char * volatile fe = "";
static const char * volatile fa = "a";
static const wchar_t * volatile wfe = L"";
static const wchar_t * volatile wfa = L"a";

TC_CASE(narrow_empty)
{
    char b[4]; int r; b[0] = 'Q'; tc_iph_count = 0;
    r = sprintf_s(b, 1, fe);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(narrow_one)
{
    char b[4]; int r; b[0] = 'Q'; tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, 1, fa);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_empty)
{
    wchar_t w[4]; int r; w[0] = L'Q'; tc_iph_count = 0;
    r = swprintf_s(w, 1, wfe);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(w[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_one)
{
    wchar_t w[4]; int r; w[0] = L'Q'; tc_iph_count = 0; errno = 0;
    r = swprintf_s(w, 1, wfa);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(w[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s); TC_LOAD_PROC(swprintf_s);
    if (!tc_p_sprintf_s || !tc_p_swprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow_empty); TC_RUN(narrow_one); TC_RUN(wide_empty); TC_RUN(wide_one);
    return tc_end();
}
