/* TC
 * id:    ct_neg_wctype_unknown_name
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: wctype
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.30.2.2.2 (wctype returns zero if the property is not valid in the current locale)
 */
#include "tc.h"

typedef unsigned short (__cdecl *wctype_t_)(const char *);
static wctype_t_ p;
static const char *volatile nm;

TC_CASE(exported) { CHECK(p != NULL); }
TC_CASE(bogus_returns_0) { if (!p) TC_SKIP("missing"); nm = "bogus"; CHECK_EQ_U(p(nm), 0); }
TC_CASE(empty_string_returns_0) { if (!p) TC_SKIP("missing"); nm = ""; CHECK_EQ_U(p(nm), 0); }
TC_CASE(prefix_alp_returns_0) { if (!p) TC_SKIP("missing"); nm = "alp"; CHECK_EQ_U(p(nm), 0); }
TC_CASE(alphaX_returns_0) { if (!p) TC_SKIP("missing"); nm = "alphaX"; CHECK_EQ_U(p(nm), 0); }
TC_CASE(valid_name_nonzero_sanity) { if (!p) TC_SKIP("missing"); nm = "alpha"; CHECK(p(nm) != 0); }

int main(void)
{
    tc_begin(); p = (wctype_t_)tc_crt_proc("wctype");
    TC_RUN(exported); TC_RUN(bogus_returns_0); TC_RUN(empty_string_returns_0); TC_RUN(prefix_alp_returns_0);
    TC_RUN(alphaX_returns_0); TC_RUN(valid_name_nonzero_sanity);
    return tc_end();
}
