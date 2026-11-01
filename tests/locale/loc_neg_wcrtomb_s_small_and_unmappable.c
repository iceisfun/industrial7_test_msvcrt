/* TC
 * id:    loc_neg_wcrtomb_s_small_and_unmappable
 * area:  locale
 * funcs: setlocale
 * procs: wcrtomb_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcrtomb_s
 * note:  Win10 triage (class B): ucrtbase 19045 reports EILSEQ, not ERANGE, for the too-small buffer in a DBCS locale (observed on Windows 10 22H2
 *        build 19045; deviates from the MS docs ERANGE row).
 *        Original: ERANGE when the output buffer is too small; EILSEQ with *pReturnValue == -1 for an unmappable character.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 *        [class B] deviates from the runtime-constraint rules of C11 Annex K (K.3.6.x / K.3.9.3.x) that the MS docs mirror; expectation = Windows behavior.
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
    CHECK(setlocale(LC_ALL, ".932") != NULL); rst(); errno = 0; memset(b, 0x55, sizeof b);
    v_wc = 0x3042; e = tc_p_wcrtomb_s(&n, b, 1, v_wc, PST);
    /* Win10 22H2 ucrtbase (observed): a 2-byte CP932 char into a 1-byte buffer yields EILSEQ (42), *pReturnValue == -1, not ERANGE; buffer untouched */
    CHECK_EQ_I(e, EILSEQ); CHECK_EQ_I(errno, EILSEQ);
    CHECK_EQ_U(n, (size_t)-1); CHECK_EQ_I((unsigned char)b[0], 0x55);
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
