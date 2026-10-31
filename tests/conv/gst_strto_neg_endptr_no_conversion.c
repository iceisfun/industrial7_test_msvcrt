/* TC
 * id:    gst_strto_neg_endptr_no_conversion
 * area:  conv
 * funcs: _strtoi64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4p8 (no conversion: endptr = nptr)
 * note:  errno-after-no-conversion lives in gst_strto_neg_errno_no_digits_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t loc;
static const char *volatile v;

TC_CASE(whitespace_only) { char *e = (char *)1; v = "   "; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v); }
TC_CASE(lone_sign) { char *e = (char *)1; v = "-"; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
                     v = "+"; e = (char *)1; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v); }
TC_CASE(digit_out_of_base) { char *e = (char *)1; v = "9"; CHECK_EQ_I(_strtoi64_l(v, &e, 8, loc), 0); CHECK_EQ_P(e, v);
                             v = "78"; e = NULL; CHECK_EQ_I(_strtoi64_l(v, &e, 8, loc), 7); CHECK_EQ_P(e, v + 1); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(whitespace_only); TC_RUN(lone_sign); TC_RUN(digit_out_of_base);
    _free_locale(loc);
    return tc_end();
}
