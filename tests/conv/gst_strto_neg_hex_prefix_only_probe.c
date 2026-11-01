/* TC
 * id:    gst_strto_neg_hex_prefix_only_probe
 * area:  conv
 * funcs: _strtoi64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.22.1.4p7 (subject sequence "0" when "0x" is not followed by a hex digit)
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): _strtoi64_l("0x", 16) and ("0xg", 16) return 0 with endptr == nptr (not nptr+1). deviates from C11 7.22.1.4p4 (the "0" before a prefix-only "0x" is not converted; endptr == nptr).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

#if TC_AT_LEAST(140)
#define EXP_END(p) (p)       /* observed Win10 22H2 ucrtbase: nothing consumed */
#else
#define EXP_END(p) ((p) + 1) /* ISO C; unverified on pre-UCRT CRTs */
#endif
static _locale_t loc;
static const char *volatile v;

TC_CASE(hex_prefix_only) { char *e = (char *)1; v = "0x"; CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 0); CHECK_EQ_P(e, EXP_END(v)); }
TC_CASE(hex_prefix_then_junk) { char *e = (char *)1; v = "0xg"; CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 0); CHECK_EQ_P(e, EXP_END(v)); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(hex_prefix_only); TC_RUN(hex_prefix_then_junk);
    _free_locale(loc);
    return tc_end();
}
