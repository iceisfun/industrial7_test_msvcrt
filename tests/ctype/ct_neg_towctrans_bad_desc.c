/* TC
 * id:    ct_neg_towctrans_bad_desc
 * area:  ctype
 * funcs:
 * procs: towctrans wctrans
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.30.3.2.1 (towctrans); MS docs towctrans
 * note:  PROBE: towctrans with a descriptor that wctrans() never returns (0 = wctrans failure value, 99, 0xFFFF) is
 *        unspecified by C; we probe that the CRT returns the argument unchanged, does not crash and does not call the
 *        invalid-parameter handler. Also probes towctrans(WEOF, toupper-descriptor) == WEOF.
 */
#include "tc.h"
#include <wchar.h>
#include <wctype.h>

typedef wint_t (__cdecl *towctrans_t)(wint_t, wint_t);
typedef wint_t (__cdecl *wctrans_t_)(const char *);
static towctrans_t p_tt; static wctrans_t_ p_wt;

TC_CASE(desc0_lower) { volatile wint_t c = L'a'; int n = tc_iph_count; CHECK_EQ_U(p_tt(c, 0), L'a'); CHECK_EQ_I(tc_iph_count, n); }
TC_CASE(desc0_upper) { volatile wint_t c = L'A'; int n = tc_iph_count; CHECK_EQ_U(p_tt(c, 0), L'A'); CHECK_EQ_I(tc_iph_count, n); }
TC_CASE(desc_big)
{
    volatile wint_t c = L'a'; int n = tc_iph_count;
    CHECK_EQ_U(p_tt(c, 99), L'a');
    CHECK_EQ_U(p_tt(c, 0xFFFF), L'a');
    CHECK_EQ_I(tc_iph_count, n);
}
TC_CASE(weof_valid_desc)
{
    wint_t d = p_wt("toupper"); int n = tc_iph_count;
    CHECK(d != 0);
    CHECK_EQ_U(p_tt(WEOF, d), WEOF);
    CHECK_EQ_I(tc_iph_count, n);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_tt = (towctrans_t)tc_crt_proc("towctrans"); p_wt = (wctrans_t_)tc_crt_proc("wctrans");
    if (!p_tt || !p_wt) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(desc0_lower); TC_RUN(desc0_upper); TC_RUN(desc_big); TC_RUN(weof_valid_desc);
    return tc_end();
}
