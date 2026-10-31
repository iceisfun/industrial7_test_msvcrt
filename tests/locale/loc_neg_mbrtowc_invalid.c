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
 * note:  EILSEQ on encoding error
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
static volatile unsigned char v_lead = 0x82, v_bad = 0x20;

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
    TC_RUN(invalid_trail_minus1); TC_RUN(errno_eilseq); TC_RUN(next_call_with_fresh_state_ok); TC_RUN(mbrlen_same);
    return tc_end();
}
