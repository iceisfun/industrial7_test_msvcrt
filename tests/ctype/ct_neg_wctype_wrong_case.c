/* TC
 * id:    ct_neg_wctype_wrong_case
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: wctype
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.30.2.2.2
 * note:  PROBES that class names are case-sensitive: wctype("ALPHA") == 0.
 */
#include "tc.h"

typedef unsigned short (__cdecl *wctype_t_)(const char *);
static const char *volatile nm = "ALPHA";

TC_CASE(wrong_case_ALPHA_returns_0)
{
    wctype_t_ p = (wctype_t_)tc_crt_proc("wctype");
    if (!p) TC_SKIP("missing");
    CHECK_EQ_U(p(nm), 0);
}
int main(void) { tc_begin(); TC_RUN(wrong_case_ALPHA_returns_0); return tc_end(); }
