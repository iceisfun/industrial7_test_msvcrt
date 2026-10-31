/* TC
 * id:    gcv_probe_atodbl_l_parse
 * area:  conv
 * funcs: _atodbl_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _atodbl, _atoflt (undocumented parsing details)
 * note:  PROBING: _atodbl_l parses "-0.0" to negative zero, skips leading whitespace and ignores trailing junk, and returns 0 with value 0.0 for a non-numeric string.
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <locale.h>

static _locale_t loc;
static int AD(_CRT_DOUBLE *d, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atodbl_l(d, b, loc); }
static int AF(_CRT_FLOAT *f, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atoflt_l(f, b, loc); }

TC_CASE(neg_zero) { _CRT_DOUBLE d; d.x = 5; CHECK_EQ_I(AD(&d, "-0.0"), 0); CHECK_BITS_D(d.x, -0.0); }
TC_CASE(space_junk) { _CRT_DOUBLE d; d.x = -1; CHECK_EQ_I(AD(&d, "  \t12.25xyz"), 0); CHECK_BITS_D(d.x, 12.25); }
TC_CASE(no_digits) { _CRT_DOUBLE d; d.x = -1; CHECK_EQ_I(AD(&d, "abc"), 0); CHECK_BITS_D(d.x, 0.0); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::setup _create_locale\n"); return 1; }
    TC_RUN(neg_zero); TC_RUN(space_junk); TC_RUN(no_digits);
    _free_locale(loc);
    return tc_end();
}
