/* TC
 * id:    loc_neg_mbrtowc_n_zero
 * area:  locale
 * funcs: 
 * procs: mbrtowc
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.29.6.3.2 (n bytes insufficient -> -2)
 * note:  PROBE: mbrtowc(&wc,"a",0,&st) returns (size_t)-2 and leaves wc untouched. Standard says -2 when n bytes are insufficient; whether the Microsoft CRT does this for n==0 is the probed point.
 */
#include "tc.h"
#include <wchar.h>
#include <string.h>

TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));
#define mbrtowc tc_p_mbrtowc

static struct { unsigned long long a, b; } st;
#define PST ((mbstate_t *)&st)
static volatile size_t v_n = 0;

TC_CASE(n0_minus2)
{
    wchar_t wc = 0x5555; char a = 'a'; size_t r;
    if (!tc_p_mbrtowc) TC_SKIP("not exported");
    memset(&st, 0, sizeof st);
    r = mbrtowc(&wc, &a, v_n, PST);
    CHECK_EQ_U(r, (size_t)-2);
}
TC_CASE(wc_untouched)
{
    wchar_t wc = 0x5555; char a = 'a';
    if (!tc_p_mbrtowc) TC_SKIP("not exported");
    memset(&st, 0, sizeof st);
    (void)mbrtowc(&wc, &a, v_n, PST);
    CHECK_EQ_U(wc, 0x5555);
}
int main(void) { tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_RUN(n0_minus2); TC_RUN(wc_untouched); return tc_end(); }
