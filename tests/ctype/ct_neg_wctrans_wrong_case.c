/* TC
 * id:    ct_neg_wctrans_wrong_case
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: wctrans
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.30.3.2.2
 * note:  PROBES that property names are case-sensitive: wctrans("TOUPPER") == 0.
 */
#include "tc.h"

typedef unsigned short (__cdecl *wctrans_p)(const char *);
static const char *volatile nm = "TOUPPER";

TC_CASE(TOUPPER_wrong_case_returns_0)
{
    wctrans_p p = (wctrans_p)tc_crt_proc("wctrans");
    if (!p) TC_SKIP("missing");
    CHECK_EQ_U(p(nm), 0);
}
int main(void) { tc_begin(); TC_RUN(TOUPPER_wrong_case_returns_0); return tc_end(); }
