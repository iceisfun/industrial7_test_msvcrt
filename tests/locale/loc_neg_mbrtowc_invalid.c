/* TC
 * id:    loc_neg_mbrtowc_invalid
 * area:  locale
 * funcs: setlocale
 * procs: mbrtowc mbrlen
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.6.3.2, 7.29.6.3.1
 * note:  EILSEQ on encoding error.
 *        Win10 triage (class A): lead 0x82 + trail 0x20 is accepted on ucrtbase 19045 (r=2, U+30FB), so the invalid-trail inputs now use a NUL
 *        trail (observed -1/EILSEQ) and the 0x20 behaviour is asserted separately as replaced_trail (deviates from C11 7.29.6.3.2). Observed on
 *        Windows 10 22H2 build 19045.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));
TC_DECL_PROC(size_t, mbrlen, (const char *, size_t, void *));
#define mbrtowc tc_p_mbrtowc
#define mbrlen tc_p_mbrlen

static struct { unsigned long long a, b; } st;
#define PST ((mbstate_t *)&st)
static volatile unsigned char v_lead = 0x82, v_bad = 0x00;   /* NUL trail byte: Win10 ucrtbase rejects it (-1/EILSEQ); a 0x20 trail is silently accepted, see replaced_trail */

TC_CASE(invalid_trail_minus1)
{
    char s[3]; wchar_t wc = 0; size_t r;
    s[0] = (char)v_lead; s[1] = (char)v_bad; s[2] = 0;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL); memset(&st, 0, sizeof st);
    r = mbrtowc(&wc, s, 2, PST);
    CHECK_EQ_U(r, (size_t)-1);
    setlocale(LC_ALL, "C");
}
TC_CASE(replaced_trail)
{
    /* Win10 22H2 ucrtbase (observed): lead 0x82 + trail 0x20 is NOT rejected by mbrtowc/mbrlen; they consume 2 bytes and mbrtowc yields U+30FB
     * (the cp932 default replacement). Deviates from C11 7.29.6.3.2 (EILSEQ). */
    char s[3]; wchar_t wc = 0; size_t r;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    s[0] = (char)0x82; s[1] = 0x20; s[2] = 0;
    CHECK(setlocale(LC_ALL, ".932") != NULL); memset(&st, 0, sizeof st); errno = 0;
    r = mbrtowc(&wc, s, 2, PST); CHECK_EQ_U(r, 2); CHECK_EQ_U(wc, 0x30FB); CHECK_EQ_I(errno, 0);
    memset(&st, 0, sizeof st);
    r = mbrlen(s, 2, PST); CHECK_EQ_U(r, 2);
    setlocale(LC_ALL, "C");
}
TC_CASE(errno_eilseq)
{
    char s[3]; wchar_t wc = 0; size_t r;
    s[0] = (char)v_lead; s[1] = (char)v_bad; s[2] = 0;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL); memset(&st, 0, sizeof st);
    errno = 0;
    r = mbrtowc(&wc, s, 2, PST);
    CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(next_call_with_fresh_state_ok)
{
    char s[3]; wchar_t wc = 0; size_t r; char a = 'a';
    s[0] = (char)v_lead; s[1] = (char)v_bad; s[2] = 0;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL); memset(&st, 0, sizeof st);
    r = mbrtowc(&wc, s, 2, PST); CHECK_EQ_U(r, (size_t)-1);
    memset(&st, 0, sizeof st);
    r = mbrtowc(&wc, &a, 1, PST);
    CHECK_EQ_U(r, 1); CHECK_EQ_U(wc, 'a');
    setlocale(LC_ALL, "C");
}
TC_CASE(mbrlen_same)
{
    char s[3]; size_t r;
    s[0] = (char)v_lead; s[1] = (char)v_bad; s[2] = 0;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL); memset(&st, 0, sizeof st);
    errno = 0;
    r = mbrlen(s, 2, PST);
    CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_LOAD_PROC(mbrlen);
    TC_RUN(invalid_trail_minus1); TC_RUN(replaced_trail); TC_RUN(errno_eilseq); TC_RUN(next_call_with_fresh_state_ok); TC_RUN(mbrlen_same);
    return tc_end();
}
