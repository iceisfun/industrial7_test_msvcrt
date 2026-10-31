/* TC
 * id:    ct_neg_wctrans_unknown_name
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: wctrans
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.30.3.2.2 (wctrans returns zero if the property is not valid in the current locale)
 */
#include "tc.h"

typedef unsigned short (__cdecl *wctrans_p)(const char *);
typedef unsigned short wint16;
static wctrans_p p;
static const char *volatile nm;

TC_CASE(exported) { CHECK(p != NULL); }
TC_CASE(bogus_returns_0) { if (!p) TC_SKIP("missing"); nm = "bogus"; CHECK_EQ_U(p(nm), 0); }
TC_CASE(empty_returns_0) { if (!p) TC_SKIP("missing"); nm = ""; CHECK_EQ_U(p(nm), 0); }
TC_CASE(toupperx_returns_0) { if (!p) TC_SKIP("missing"); nm = "toupperx"; CHECK_EQ_U(p(nm), 0); }
TC_CASE(valid_toupper_nonzero_sanity) { if (!p) TC_SKIP("missing"); nm = "toupper"; CHECK(p(nm) != 0); }

int main(void)
{
    tc_begin(); p = (wctrans_p)tc_crt_proc("wctrans");
    TC_RUN(exported); TC_RUN(bogus_returns_0); TC_RUN(empty_returns_0); TC_RUN(toupperx_returns_0);
    TC_RUN(valid_toupper_nonzero_sanity);
    return tc_end();
}
