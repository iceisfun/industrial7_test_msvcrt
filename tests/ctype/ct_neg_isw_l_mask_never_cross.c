/* TC
 * id:    ct_neg_isw_l_mask_never_cross
 * area:  ctype
 * kind:  negative
 * funcs: iswctype iswalpha iswdigit iswupper iswlower iswpunct iswspace iswxdigit iswcntrl
 * procs: wctype
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.30.2.2.1 iswctype, 7.30.2.2.2 wctype; MS docs iswctype, wctype
 */
#include "tc.h"
#include <wctype.h>

typedef unsigned short (__cdecl *wctype_p)(const char *);
static wctype_p p_wctype;
static volatile unsigned short vc;

static void sweep(const char *name, int (__cdecl *fn)(wint_t))
{
    unsigned short m; int c;
    if (!p_wctype) TC_SKIP("wctype missing");
    m = p_wctype(name);
    CHECK(m != 0);
    for (c = 0; c < 128; c++) {
        vc = (unsigned short)c;
        if ((iswctype(vc, m) != 0) != (fn(vc) != 0)) { CHECK_EQ_I(c, -1); break; }
    }
}
TC_CASE(alpha) { sweep("alpha", iswalpha); }
TC_CASE(digit) { sweep("digit", iswdigit); }
TC_CASE(upper) { sweep("upper", iswupper); }
TC_CASE(lower) { sweep("lower", iswlower); }
TC_CASE(punct_space) { sweep("punct", iswpunct); sweep("space", iswspace); }
TC_CASE(xdigit_cntrl) { sweep("xdigit", iswxdigit); sweep("cntrl", iswcntrl); }
TC_CASE(digit_mask_rejects_letters)
{
    unsigned short m; int c;
    if (!p_wctype) TC_SKIP("wctype missing");
    m = p_wctype("digit");
    for (c = 'A'; c <= 'z'; c++) { vc = (unsigned short)c; CHECK_EQ_I(iswctype(vc, m), 0); }
}
TC_CASE(upper_mask_rejects_lowercase)
{
    unsigned short m; int c;
    if (!p_wctype) TC_SKIP("wctype missing");
    m = p_wctype("upper");
    for (c = 'a'; c <= 'z'; c++) { vc = (unsigned short)c; CHECK_EQ_I(iswctype(vc, m), 0); }
}
int main(void)
{
    tc_begin();
    p_wctype = (wctype_p)tc_crt_proc("wctype");
    TC_RUN(alpha); TC_RUN(digit); TC_RUN(upper); TC_RUN(lower); TC_RUN(punct_space); TC_RUN(xdigit_cntrl);
    TC_RUN(digit_mask_rejects_letters); TC_RUN(upper_mask_rejects_lowercase);
    return tc_end();
}
