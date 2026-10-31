/* TC
 * id:    ct_wctype_names
 * area:  ctype
 * funcs: 
 * procs: wctype iswctype
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.30.2.2.2 wctype, 7.30.2.2.1 iswctype
 * note:  Descriptor values are not asserted, only nonzero/behaviour. The C99 "blank" class is probed separately (ct_probe_wctype_blank).
 */
#include "tc.h"
#include <wctype.h>

typedef wctype_t (__cdecl *wctype_f)(const char *);
typedef int (__cdecl *iswctype_f)(wint_t, wctype_t);
static wctype_f p_wctype; static iswctype_f p_iswctype;

TC_CASE(all_11_names_nonzero)
{
    static const char *const n[] = {"alnum","alpha","cntrl","digit","graph","lower","print","punct","space","upper","xdigit"};
    int i;
    CHECK(p_wctype != NULL && p_iswctype != NULL);
    if (!p_wctype) return;
    for (i = 0; i < 11; i++) {
        if (!p_wctype(n[i])) { tc_fail_expr(__FILE__, __LINE__, n[i]); }
    }
}
TC_CASE(iswctype_alpha_on_A) { if (!p_wctype) TC_SKIP("none"); CHECK(p_iswctype(L'A', p_wctype("alpha")) != 0); }
TC_CASE(iswctype_digit_on_7) { if (!p_wctype) TC_SKIP("none"); CHECK(p_iswctype(L'7', p_wctype("digit")) != 0); }
TC_CASE(iswctype_space_on_tab) { if (!p_wctype) TC_SKIP("none"); CHECK(p_iswctype(L'\t', p_wctype("space")) != 0); }
TC_CASE(iswctype_alpha_not_on_digit) { if (!p_wctype) TC_SKIP("none"); CHECK_EQ_I(p_iswctype(L'7', p_wctype("alpha")), 0); }
int main(void)
{
    tc_begin();
    p_wctype = (wctype_f)tc_crt_proc("wctype"); p_iswctype = (iswctype_f)tc_crt_proc("iswctype");
    TC_RUN(all_11_names_nonzero); TC_RUN(iswctype_alpha_on_A); TC_RUN(iswctype_digit_on_7);
    TC_RUN(iswctype_space_on_tab); TC_RUN(iswctype_alpha_not_on_digit);
    return tc_end();
}
