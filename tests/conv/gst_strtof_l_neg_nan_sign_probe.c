/* TC
 * id:    gst_strtof_l_neg_nan_sign_probe
 * area:  conv
 * funcs: _strtof_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3p3 (sign of NaN is implementation-defined)
 * note:  Probes that "-nan" yields a NaN with the sign bit set and "nan" one with it clear. Not documented by Microsoft.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t loc;
static const char *volatile v;

TC_CASE(neg_nan_sign) { float f; v = "-nan"; f = _strtof_l(v, NULL, loc); CHECK(f != f); CHECK((tc_fbits(f) >> 31) == 1u); }
TC_CASE(pos_nan_sign) { float f; v = "nan"; f = _strtof_l(v, NULL, loc); CHECK(f != f); CHECK((tc_fbits(f) >> 31) == 0u); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(neg_nan_sign); TC_RUN(pos_nan_sign);
    _free_locale(loc);
    return tc_end();
}
