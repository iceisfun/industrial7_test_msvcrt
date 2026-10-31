/* TC
 * id:    ct_probe_iswctype_zero_desc
 * area:  ctype
 * funcs: 
 * procs: wctype iswctype
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.30.2.2.1 (desc must come from wctype; zero is unspecified)
 * note:  PROBE: iswctype(c, 0) with the "unknown class" descriptor returns 0 without crashing or calling the
 *        invalid-parameter handler (an invalid-parameter handler is installed where available).
 */
#include "tc.h"
#include <wctype.h>

typedef wctype_t (__cdecl *wctype_f)(const char *);
typedef int (__cdecl *iswctype_f)(wint_t, wctype_t);

TC_CASE(zero_desc_false)
{
    iswctype_f p = (iswctype_f)tc_crt_proc("iswctype");
    wctype_f w = (wctype_f)tc_crt_proc("wctype");
    wctype_t d;
    CHECK(p != NULL && w != NULL); if (!p || !w) return;
    tc_iph_install();
    d = w("bogus");
    CHECK_EQ_I(p(L'A', d), 0);
    CHECK_EQ_I(p(L'7', d), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(zero_desc_false);
    return tc_end();
}
