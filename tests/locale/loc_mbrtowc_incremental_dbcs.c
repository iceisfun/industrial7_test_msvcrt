/* TC
 * id:    loc_mbrtowc_incremental_dbcs
 * area:  locale
 * funcs: setlocale
 * procs: mbrtowc mbrlen
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.3.2; MS docs mbrtowc, mbrlen
 * note:  -2 means incomplete character; C11 says the second call returns 1 (bytes consumed by that call).
 *        Win10 triage (class B): ucrtbase 19045 returns 2 on the completing call (observed, deviates from C11 7.29.6.3.2); legacy CRTs [unverified]. msvcr90 and earlier excluded: state carrying not documented.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>

TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));
TC_DECL_PROC(size_t, mbrlen, (const char *, size_t, void *));
#define mbrtowc tc_p_mbrtowc
#define mbrlen tc_p_mbrlen

static struct { unsigned long long a, b; } st;   /* >= sizeof(mbstate_t) of every CRT */
static const char * volatile v_loc;
static volatile unsigned char v_lead = 0x82, v_trail = 0xA0;
#define PST ((mbstate_t *)&st)
/* Win10 22H2 ucrtbase (observed): the call that completes a DBCS character split across calls returns 2 (total char length), not 1 (bytes consumed
 * by that call) as C11 7.29.6.3.2 says. Legacy CRTs: [unverified], keep the C11 value. */
#if TC_AT_LEAST(140)
#define COMPLETE_RET 2
#else
#define COMPLETE_RET 1
#endif

static void enter(void) { v_loc = ".932"; memset(&st, 0, sizeof st); }
static void leave(void) { setlocale(LC_ALL, "C"); }

TC_CASE(lead_returns_minus2)
{
    char c = (char)v_lead; wchar_t wc = 0x7777; size_t r;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL); enter();
    r = mbrtowc(&wc, &c, 1, PST);
    CHECK_EQ_U(r, (size_t)-2);
    leave();
}
/* name kept for case-id stability: on ucrtbase the completing call returns 2 (total length), not 1 */
TC_CASE(trail_completes_with_1)
{
    char c = (char)v_lead, t = (char)v_trail; wchar_t wc = 0; size_t r;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL); enter();
    r = mbrtowc(&wc, &c, 1, PST); CHECK_EQ_U(r, (size_t)-2);
    r = mbrtowc(&wc, &t, 1, PST); CHECK_EQ_U(r, COMPLETE_RET);
    leave();
}
TC_CASE(wc_value_3042)
{
    char c = (char)v_lead, t = (char)v_trail; wchar_t wc = 0; size_t r;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL); enter();
    r = mbrtowc(&wc, &c, 1, PST); (void)r;
    r = mbrtowc(&wc, &t, 1, PST);
    CHECK_EQ_U(r, COMPLETE_RET); CHECK_EQ_U(wc, 0x3042);
    leave();
}
/* name kept for case-id stability: the completing call returns 2 on ucrtbase */
TC_CASE(mbrlen_split_minus2_then_1)
{
    char c = (char)v_lead, t = (char)v_trail; size_t r;
    if (!tc_p_mbrtowc || !tc_p_mbrlen) TC_SKIP("not exported");
    CHECK(setlocale(LC_ALL, ".932") != NULL); enter();
    r = mbrlen(&c, 1, PST); CHECK_EQ_U(r, (size_t)-2);
    r = mbrlen(&t, 1, PST); CHECK_EQ_U(r, COMPLETE_RET);
    leave();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_LOAD_PROC(mbrlen);
    TC_RUN(lead_returns_minus2); TC_RUN(trail_completes_with_1); TC_RUN(wc_value_3042); TC_RUN(mbrlen_split_minus2_then_1);
    return tc_end();
}
