/* TC
 * id:    ct_neg_extreme_values_ucrt
 * area:  ctype
 * kind:  negative
 * funcs: isalpha isdigit isspace toupper tolower isleadbyte _isctype towupper
 * procs: iswalpha
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.4p1 (undefined); MS docs _isctype, isleadbyte
 * note:  PROBES: INT_MIN / INT_MAX must not crash the process; classes return 0 and case maps return the argument.
 *        _isctype is probed with INT_MIN only (low byte 0): for other out-of-range values _isctype may classify the
 *        low byte via GetStringTypeA, so INT_MAX (low byte 0xFF, alpha in cp1252) is not asserted.
 *        wint_t is 16-bit so 0x7FFFFFFF converts to 0xFFFF (WEOF) before the call.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>
#include <limits.h>

static volatile int vc;
typedef int (__cdecl *isw_t)(unsigned short);

TC_CASE(isalpha_INT_MIN_INT_MAX_zero)
{
    vc = INT_MIN; CHECK_EQ_I(isalpha(vc), 0); CHECK_EQ_I(isdigit(vc), 0); CHECK_EQ_I(isspace(vc), 0);
    vc = INT_MAX; CHECK_EQ_I(isalpha(vc), 0); CHECK_EQ_I(isdigit(vc), 0); CHECK_EQ_I(isspace(vc), 0);
}
TC_CASE(toupper_tolower_INT_MIN_INT_MAX_identity)
{
    vc = INT_MIN; CHECK_EQ_I(toupper(vc), INT_MIN); CHECK_EQ_I(tolower(vc), INT_MIN);
    vc = INT_MAX; CHECK_EQ_I(toupper(vc), INT_MAX); CHECK_EQ_I(tolower(vc), INT_MAX);
}
TC_CASE(isleadbyte_INT_MIN_INT_MAX_zero)
{
    vc = INT_MIN; CHECK_EQ_I(isleadbyte(vc), 0);
    vc = INT_MAX; CHECK_EQ_I(isleadbyte(vc), 0);
}
TC_CASE(_isctype_INT_MIN_INT_MAX_zero)
{
    vc = INT_MIN; CHECK_EQ_I(_isctype(vc, _ALPHA), 0);
    /* INT_MAX deliberately not asserted: low byte 0xFF may be classified via GetStringTypeA in the ANSI code page */
}
TC_CASE(iswalpha_towupper_0x7FFFFFFF_truncated_wint)
{
    isw_t f = (isw_t)tc_crt_proc("iswalpha"); wint_t w; vc = 0x7FFFFFFF; w = (wint_t)vc;
    CHECK(f != NULL);
    if (f) CHECK_EQ_I(f(w), 0);
    CHECK_EQ_U(towupper(w), 0xFFFF);
}

int main(void)
{
    tc_begin();
    TC_RUN(isalpha_INT_MIN_INT_MAX_zero); TC_RUN(toupper_tolower_INT_MIN_INT_MAX_identity);
    TC_RUN(isleadbyte_INT_MIN_INT_MAX_zero); TC_RUN(_isctype_INT_MIN_INT_MAX_zero);
    TC_RUN(iswalpha_towupper_0x7FFFFFFF_truncated_wint);
    return tc_end();
}
