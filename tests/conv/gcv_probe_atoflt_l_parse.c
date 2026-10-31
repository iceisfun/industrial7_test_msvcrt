/* TC
 * id:    gcv_probe_atoflt_l_parse
 * area:  conv
 * funcs: _atoflt_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _atodbl, _atoflt (undocumented parsing details)
 * note:  PROBING: _atoflt_l of "3.4028235e38" (rounds to FLT_MAX), no-digit string returns 0/0.0f, trailing junk ignored.
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <locale.h>

static _locale_t loc;
static int AD(_CRT_DOUBLE *d, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atodbl_l(d, b, loc); }
static int AF(_CRT_FLOAT *f, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atoflt_l(f, b, loc); }

TC_CASE(flt_max) { _CRT_FLOAT f; f.f = -1; CHECK_EQ_I(AF(&f, "3.4028235e38"), 0); CHECK_BITS_F(f.f, FLT_MAX); }
TC_CASE(no_digits) { _CRT_FLOAT f; f.f = -1; CHECK_EQ_I(AF(&f, "xyz"), 0); CHECK_BITS_F(f.f, 0.0f); }
TC_CASE(junk) { _CRT_FLOAT f; f.f = -1; CHECK_EQ_I(AF(&f, "4.5abc"), 0); CHECK_BITS_F(f.f, 4.5f); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::setup _create_locale\n"); return 1; }
    TC_RUN(flt_max); TC_RUN(no_digits); TC_RUN(junk);
    _free_locale(loc);
    return tc_end();
}
