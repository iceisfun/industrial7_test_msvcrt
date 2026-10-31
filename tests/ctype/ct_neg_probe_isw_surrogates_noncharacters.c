/* TC
 * id:    ct_neg_probe_isw_surrogates_noncharacters
 * area:  ctype
 * funcs: towupper towlower
 * procs: iswalpha iswupper iswlower iswdigit iswpunct iswalnum
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs iswalpha etc.; Unicode surrogates/noncharacters have no letter/case properties
 * note:  PROBE: lone surrogates and noncharacters are not alpha/alnum/digit/upper/lower and map to themselves. iswpunct is not asserted.
 */
#include "tc.h"
#include <wctype.h>

typedef int (__cdecl *isw_t)(wint_t);
static isw_t p_alpha, p_upper, p_lower, p_digit, p_punct, p_alnum;
#define iswalpha p_alpha
#define iswupper p_upper
#define iswlower p_lower
#define iswdigit p_digit
#define iswpunct p_punct
#define iswalnum p_alnum

static const unsigned short vals[] = {0xD800, 0xDBFF, 0xDC00, 0xDFFF, 0xFFFE, 0xFDD0};
static void classes_range(int lo, int hi)
{
    int i;
    for (i = lo; i < hi; i++) {
        volatile wint_t c = vals[i];
        CHECK(iswalpha(c) == 0); CHECK(iswalnum(c) == 0); CHECK(iswdigit(c) == 0);
        CHECK(iswupper(c) == 0); CHECK(iswlower(c) == 0);
    }
}
TC_CASE(classes_surrogates) { classes_range(0, 4); }
TC_CASE(classes_noncharacters) { classes_range(4, 6); }
TC_CASE(case_identity)
{
    int i;
    for (i = 0; i < 6; i++) {
        volatile wint_t c = vals[i];
        CHECK_EQ_U(towupper(c), vals[i]); CHECK_EQ_U(towlower(c), vals[i]);
    }
}
TC_CASE(callable_all) { volatile wint_t c = 0xD800; (void)iswpunct(c); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); tc_iph_install();
    p_alpha = (isw_t)tc_crt_proc("iswalpha"); p_upper = (isw_t)tc_crt_proc("iswupper"); p_lower = (isw_t)tc_crt_proc("iswlower");
    p_digit = (isw_t)tc_crt_proc("iswdigit"); p_punct = (isw_t)tc_crt_proc("iswpunct"); p_alnum = (isw_t)tc_crt_proc("iswalnum");
    if (!p_alpha || !p_upper || !p_lower || !p_digit || !p_punct || !p_alnum) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
 TC_RUN(classes_surrogates); TC_RUN(classes_noncharacters); TC_RUN(case_identity); TC_RUN(callable_all); return tc_end(); }
