/* TC
 * id:    ct_neg_l_variants_null_locale
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: _isalpha_l _isdigit_l _isctype_l _toupper_l _tolower_l _towupper_l _iswalpha_l
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _isalpha_l etc. (locale parameter), parameter validation
 * note:  PROBES the undocumented hypothesis that a NULL _locale_t means "use the current locale" and does not
 *        invoke the invalid-parameter handler.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>

TC_DECL_PROC(int, _isalpha_l, (int, void *));
TC_DECL_PROC(int, _isdigit_l, (int, void *));
TC_DECL_PROC(int, _isctype_l, (int, int, void *));
TC_DECL_PROC(int, _toupper_l, (int, void *));
TC_DECL_PROC(int, _towupper_l, (unsigned short, void *));
TC_DECL_PROC(int, _iswalpha_l, (unsigned short, void *));
static volatile int vc;

TC_CASE(isalpha_l_a_NULL_nonzero) { if (!tc_p__isalpha_l) TC_SKIP("missing"); vc = 'a'; CHECK(tc_p__isalpha_l(vc, NULL) != 0); }
TC_CASE(toupper_l_a_NULL_is_A) { if (!tc_p__toupper_l) TC_SKIP("missing"); vc = 'a'; CHECK_EQ_I(tc_p__toupper_l(vc, NULL), 'A'); }
TC_CASE(isctype_l_7_DIGIT_NULL_nonzero) { if (!tc_p__isctype_l) TC_SKIP("missing"); vc = '7'; CHECK(tc_p__isctype_l(vc, _DIGIT, NULL) != 0); }
TC_CASE(iswalpha_l_a_NULL_nonzero) { if (!tc_p__iswalpha_l) TC_SKIP("missing"); vc = 'a'; CHECK(tc_p__iswalpha_l((unsigned short)vc, NULL) != 0); }
TC_CASE(towupper_l_a_NULL_is_A) { if (!tc_p__towupper_l) TC_SKIP("missing"); vc = 'a'; CHECK_EQ_I(tc_p__towupper_l((unsigned short)vc, NULL) & 0xFFFF, 'A'); }
TC_CASE(no_iph_count_change)
{
    tc_iph_count = 0; vc = 'a';
    if (tc_p__isalpha_l) (void)tc_p__isalpha_l(vc, NULL);
    if (tc_p__isdigit_l) (void)tc_p__isdigit_l(vc, NULL);
    if (tc_p__isctype_l) (void)tc_p__isctype_l(vc, _ALPHA, NULL);
    if (tc_p__toupper_l) (void)tc_p__toupper_l(vc, NULL);
    if (tc_p__towupper_l) (void)tc_p__towupper_l((unsigned short)vc, NULL);
    if (tc_p__iswalpha_l) (void)tc_p__iswalpha_l((unsigned short)vc, NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_isalpha_l); TC_LOAD_PROC(_isdigit_l); TC_LOAD_PROC(_isctype_l); TC_LOAD_PROC(_toupper_l);
    TC_LOAD_PROC(_towupper_l); TC_LOAD_PROC(_iswalpha_l);
    tc_iph_install();
    TC_RUN(isalpha_l_a_NULL_nonzero); TC_RUN(toupper_l_a_NULL_is_A); TC_RUN(isctype_l_7_DIGIT_NULL_nonzero);
    TC_RUN(iswalpha_l_a_NULL_nonzero); TC_RUN(towupper_l_a_NULL_is_A); TC_RUN(no_iph_count_change);
    return tc_end();
}
