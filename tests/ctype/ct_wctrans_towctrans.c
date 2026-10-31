/* TC
 * id:    ct_wctrans_towctrans
 * area:  ctype
 * funcs: 
 * procs: wctrans towctrans
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.30.3.2 wctrans, 7.30.3.1 towctrans
 * note:  Descriptor values not asserted.
 */
#include "tc.h"
#include <wctype.h>

typedef wctrans_t (__cdecl *wctrans_f)(const char *);
typedef wint_t (__cdecl *towctrans_f)(wint_t, wctrans_t);
static wctrans_f p_wctrans; static towctrans_f p_towctrans;

TC_CASE(wctrans_toupper_nonzero) { CHECK(p_wctrans && p_towctrans); if (!p_wctrans) return; CHECK(p_wctrans("toupper") != 0); }
TC_CASE(wctrans_tolower_nonzero) { if (!p_wctrans) TC_SKIP("none"); CHECK(p_wctrans("tolower") != 0); }
TC_CASE(towctrans_a_toupper_is_A) { if (!p_wctrans) TC_SKIP("none"); CHECK_EQ_U(p_towctrans(L'a', p_wctrans("toupper")), L'A'); }
TC_CASE(towctrans_A_tolower_is_a) { if (!p_wctrans) TC_SKIP("none"); CHECK_EQ_U(p_towctrans(L'A', p_wctrans("tolower")), L'a'); }
TC_CASE(towctrans_nonletter_unchanged)
{
    if (!p_wctrans) TC_SKIP("none");
    CHECK_EQ_U(p_towctrans(L'5', p_wctrans("toupper")), L'5');
    CHECK_EQ_U(p_towctrans(L'!', p_wctrans("tolower")), L'!');
}
int main(void)
{
    tc_begin();
    p_wctrans = (wctrans_f)tc_crt_proc("wctrans"); p_towctrans = (towctrans_f)tc_crt_proc("towctrans");
    TC_RUN(wctrans_toupper_nonzero); TC_RUN(wctrans_tolower_nonzero); TC_RUN(towctrans_a_toupper_is_A);
    TC_RUN(towctrans_A_tolower_is_a); TC_RUN(towctrans_nonletter_unchanged);
    return tc_end();
}
