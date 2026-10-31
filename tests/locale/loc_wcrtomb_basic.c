/* TC
 * id:    loc_wcrtomb_basic
 * area:  locale
 * funcs: setlocale
 * procs: wcrtomb
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.3.3; MS docs wcrtomb
 * note:  
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>

TC_DECL_PROC(size_t, wcrtomb, (char *, wchar_t, void *));
#define wcrtomb tc_p_wcrtomb

static struct { unsigned long long a, b; } st;
#define PST ((mbstate_t *)&st)
static volatile wchar_t v_wc;

TC_CASE(ascii)
{
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    char b[8]; size_t r; memset(b, 0x55, sizeof b); memset(&st, 0, sizeof st);
    v_wc = L'A'; r = wcrtomb(b, v_wc, PST);
    CHECK_EQ_U(r, 1); CHECK_EQ_I(b[0], 'A');
}
TC_CASE(nul)
{
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    char b[8]; size_t r; memset(b, 0x55, sizeof b); memset(&st, 0, sizeof st);
    v_wc = 0; r = wcrtomb(b, v_wc, PST);
    CHECK_EQ_U(r, 1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I((unsigned char)b[1], 0x55);
}
TC_CASE(euro_1252)
{
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    char b[8]; size_t r; memset(b, 0, sizeof b); memset(&st, 0, sizeof st);
    CHECK(setlocale(LC_ALL, ".1252") != NULL);
    v_wc = 0x20AC; r = wcrtomb(b, v_wc, PST);
    CHECK_EQ_U(r, 1); CHECK_EQ_U((unsigned char)b[0], 0x80);
    setlocale(LC_ALL, "C");
}
TC_CASE(dbcs_2_bytes)
{
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    char b[8]; size_t r; memset(b, 0, sizeof b); memset(&st, 0, sizeof st);
    CHECK(setlocale(LC_ALL, ".932") != NULL);
    v_wc = 0x3042; r = wcrtomb(b, v_wc, PST);
    CHECK_EQ_U(r, 2); CHECK_EQ_U((unsigned char)b[0], 0x82); CHECK_EQ_U((unsigned char)b[1], 0xA0);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcrtomb);
    TC_RUN(ascii); TC_RUN(nul); TC_RUN(euro_1252); TC_RUN(dbcs_2_bytes);
    return tc_end();
}
