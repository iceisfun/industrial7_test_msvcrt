/* TC
 * id:    loc_neg_mbrtowc_n_zero
 * area:  locale
 * funcs: 
 * procs: mbrtowc
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.29.6.3.2 (n bytes insufficient -> -2)
 * note:  Win10 triage (class B, probe -> observed): ucrtbase mbrtowc(&wc,"a",0,&st) returns 0 and stores L'\0' in wc; observed on Windows 10 22H2
 *        build 19045, deviates from C11 7.29.6.3.2 (-2, wc untouched).
 *        Original PROBE: mbrtowc(&wc,"a",0,&st) returns (size_t)-2 and leaves wc untouched. Standard says -2 when n bytes are insufficient; whether the Microsoft CRT does this for n==0 is the probed point.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 */
#include "tc.h"
#include <wchar.h>
#include <string.h>

TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));
#define mbrtowc tc_p_mbrtowc

static struct { unsigned long long a, b; } st;
#define PST ((mbstate_t *)&st)
static volatile size_t v_n = 0;

/* name kept for case-id stability: Windows returns 0 for n == 0, not (size_t)-2 */
TC_CASE(n0_minus2)
{
    wchar_t wc = 0x5555; char a = 'a'; size_t r;
    if (!tc_p_mbrtowc) TC_SKIP("not exported");
    memset(&st, 0, sizeof st);
    r = mbrtowc(&wc, &a, v_n, PST);
    CHECK_EQ_U(r, 0);   /* observed on Win10 22H2 ucrtbase: 0, not -2 (deviates from C11 7.29.6.3.2) */
}
/* name kept for case-id stability: Windows WRITES wc (0) for n == 0 */
TC_CASE(wc_untouched)
{
    wchar_t wc = 0x5555; char a = 'a';
    if (!tc_p_mbrtowc) TC_SKIP("not exported");
    memset(&st, 0, sizeof st);
    (void)mbrtowc(&wc, &a, v_n, PST);
    CHECK_EQ_U(wc, 0);  /* observed: *pwc is written with 0 even though n == 0 (deviates from C11 7.29.6.3.2) */
}
int main(void) { tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_RUN(n0_minus2); TC_RUN(wc_untouched); return tc_end(); }
