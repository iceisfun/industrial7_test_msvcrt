/* TC
 * id:    ct_probe_wctype_blank
 * area:  ctype
 * funcs: 
 * procs: wctype iswctype
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   C99 7.25.2.2.2 (wctype "blank" required)
 * note:  PROBE: whether the Microsoft wctype() recognises the C99 class name "blank" (returns nonzero) and
 *        iswctype(L' ', blank) is nonzero while iswctype(L'A', blank) is zero.
 *        Observed Win10 22H2 ucrtbase: wctype("blank") == 0x40; iswctype(L' ')!=0 but iswctype(L'\t', blank) == 0 although
 *        iswblank(L'\t') != 0; deviates from C11 7.30.2.2.2/7.30.2.1.3 (TAB is a blank). Stable.
 */
#include "tc.h"
#include <wctype.h>

typedef wctype_t (__cdecl *wctype_f)(const char *);
typedef int (__cdecl *iswctype_f)(wint_t, wctype_t);
static wctype_f p_wctype; static iswctype_f p_iswctype;

TC_CASE(wctype_blank_nonzero) { CHECK(p_wctype != NULL); if (!p_wctype) return; CHECK(p_wctype("blank") != 0); }
TC_CASE(iswctype_blank_space_and_A)
{
    wctype_t d;
    if (!p_wctype || !p_iswctype) TC_SKIP("none");
    d = p_wctype("blank");
    if (!d) TC_SKIP("blank unrecognised");
    CHECK(p_iswctype(L' ', d) != 0);
    CHECK_EQ_I(p_iswctype(L'\t', d), 0);   /* observed: TAB is not in the wctype("blank") class */
    CHECK_EQ_I(p_iswctype(L'A', d), 0);
}
int main(void)
{
    tc_begin();
    p_wctype = (wctype_f)tc_crt_proc("wctype"); p_iswctype = (iswctype_f)tc_crt_proc("iswctype");
    TC_RUN(wctype_blank_nonzero); TC_RUN(iswctype_blank_space_and_A);
    return tc_end();
}
