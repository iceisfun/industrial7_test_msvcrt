/* TC
 * id:    loc_neg_wcrtomb_s_small_and_unmappable
 * area:  locale
 * funcs: setlocale
 * procs: wcrtomb_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcrtomb_s
 * note:  ERANGE when the output buffer is too small; EILSEQ with *pReturnValue == -1 for an unmappable character.
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

TC_CASE(erange_dbcs)
{
    char b[8]; size_t n = 99; errno_t e;
    if (!tc_p_wcrtomb_s || !tc_iph_install()) TC_SKIP("n/a");
    CHECK(setlocale(LC_ALL, ".932") != NULL); rst(); errno = 0;
    v_wc = 0x3042; e = tc_p_wcrtomb_s(&n, b, 1, v_wc, PST);
    CHECK_EQ_I(e, ERANGE); CHECK_EQ_I(errno, ERANGE);
    setlocale(LC_ALL, "C");
}
TC_CASE(eilseq_cyrillic)
{
    char b[8]; size_t n = 99; errno_t e;
    if (!tc_p_wcrtomb_s || !tc_iph_install()) TC_SKIP("n/a");
    CHECK(setlocale(LC_ALL, ".1252") != NULL); rst(); errno = 0;
    v_wc = 0x0416; e = tc_p_wcrtomb_s(&n, b, sizeof b, v_wc, PST);
    CHECK_EQ_I(e, EILSEQ); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(pReturn_minus1)
{
    char b[8]; size_t n = 99;
    if (!tc_p_wcrtomb_s || !tc_iph_install()) TC_SKIP("n/a");
    CHECK(setlocale(LC_ALL, ".1252") != NULL); rst();
    v_wc = 0x0416; tc_p_wcrtomb_s(&n, b, sizeof b, v_wc, PST);
    CHECK_EQ_U(n, (size_t)-1);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcrtomb_s);
    TC_RUN(erange_dbcs); TC_RUN(eilseq_cyrillic); TC_RUN(pReturn_minus1);
    return tc_end();
}
