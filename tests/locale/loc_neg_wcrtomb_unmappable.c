/* TC
 * id:    loc_neg_wcrtomb_unmappable
 * area:  locale
 * funcs: setlocale
 * procs: wcrtomb
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.6.3.3; MS docs wcrtomb
 * note:  State remains usable afterwards (case afterwards_ascii_ok).
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

TC_DECL_PROC(size_t, wcrtomb, (char *, wchar_t, void *));
#define wcrtomb tc_p_wcrtomb

static struct { unsigned long long a, b; } st;
#define PST ((mbstate_t *)&st)
static volatile wchar_t v_wc;

TC_CASE(1252_cyrillic)
{
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    char b[8]; size_t r; memset(&st, 0, sizeof st);
    CHECK(setlocale(LC_ALL, ".1252") != NULL);
    v_wc = 0x0416; r = wcrtomb(b, v_wc, PST);
    CHECK_EQ_U(r, (size_t)-1);
    setlocale(LC_ALL, "C");
}
TC_CASE(errno_eilseq)
{
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    char b[8]; size_t r; memset(&st, 0, sizeof st);
    CHECK(setlocale(LC_ALL, ".1252") != NULL);
    errno = 0; v_wc = 0x0416; r = wcrtomb(b, v_wc, PST);
    CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(c_locale_above_ff)
{
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    char b[8]; size_t r; memset(&st, 0, sizeof st);
    CHECK(setlocale(LC_ALL, "C") != NULL);
    errno = 0; v_wc = 0x0100; r = wcrtomb(b, v_wc, PST);
    CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(afterwards_ascii_ok)
{
    if (!tc_p_wcrtomb) TC_SKIP("not exported");
    char b[8]; size_t r; memset(&st, 0, sizeof st);
    CHECK(setlocale(LC_ALL, ".1252") != NULL);
    v_wc = 0x0416; r = wcrtomb(b, v_wc, PST); CHECK_EQ_U(r, (size_t)-1);
    v_wc = L'Z'; r = wcrtomb(b, v_wc, PST);
    CHECK_EQ_U(r, 1); CHECK_EQ_I(b[0], 'Z');
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcrtomb);
    TC_RUN(1252_cyrillic); TC_RUN(errno_eilseq); TC_RUN(c_locale_above_ff); TC_RUN(afterwards_ascii_ok);
    return tc_end();
}
