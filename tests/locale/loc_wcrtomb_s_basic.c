/* TC
 * id:    loc_wcrtomb_s_basic
 * area:  locale
 * funcs: setlocale
 * procs: wcrtomb_s
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wcrtomb_s
 * note:  pReturn counts bytes written.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

static struct { unsigned long long a, b; } st;
#define PST ((void *)&st)
static void rst(void) { memset(&st, 0, sizeof st); }

TC_DECL_PROC(errno_t, wcrtomb_s, (size_t *, char *, size_t, wchar_t, void *));
static volatile wchar_t v_wc;

TC_CASE(ascii_count1)
{
    char b[8]; size_t n = 99; errno_t e; rst(); memset(b, 0, sizeof b);
    if (!tc_p_wcrtomb_s) TC_SKIP("not exported");
    v_wc = L'A'; e = tc_p_wcrtomb_s(&n, b, sizeof b, v_wc, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(n, 1); CHECK_EQ_I(b[0], 'A');
}
TC_CASE(dbcs_count2)
{
    char b[8]; size_t n = 99; errno_t e; rst(); memset(b, 0, sizeof b);
    if (!tc_p_wcrtomb_s) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL);
    v_wc = 0x3042; e = tc_p_wcrtomb_s(&n, b, sizeof b, v_wc, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(n, 2); CHECK_EQ_U((unsigned char)b[0], 0x82); CHECK_EQ_U((unsigned char)b[1], 0xA0);
    setlocale(LC_ALL, "C");
}
TC_CASE(nul_count1)
{
    char b[8]; size_t n = 99; errno_t e; rst(); memset(b, 0x55, sizeof b);
    if (!tc_p_wcrtomb_s) TC_SKIP("not exported");
    v_wc = 0; e = tc_p_wcrtomb_s(&n, b, sizeof b, v_wc, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(n, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(euro_1252)
{
    char b[8]; size_t n = 99; errno_t e; rst(); memset(b, 0, sizeof b);
    if (!tc_p_wcrtomb_s) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".1252") != NULL);
    v_wc = 0x20AC; e = tc_p_wcrtomb_s(&n, b, sizeof b, v_wc, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(n, 1); CHECK_EQ_U((unsigned char)b[0], 0x80);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcrtomb_s);
    TC_RUN(ascii_count1); TC_RUN(dbcs_count2); TC_RUN(nul_count1); TC_RUN(euro_1252);
    return tc_end();
}
