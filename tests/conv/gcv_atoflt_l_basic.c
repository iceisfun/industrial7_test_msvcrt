/* TC
 * id:    gcv_atoflt_l_basic
 * area:  conv
 * funcs: _atoflt_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _atoflt, _atoflt_l (return 0, _UNDERFLOW or _OVERFLOW)
 * note:  FLT_MAX boundary / no-digit / trailing-junk cases moved to gcv_probe_atoflt_l_parse (undocumented, rounding path may differ per CRT).
 *        msvcr80 excluded: its import library lacks _atoflt_l (and _atoflt).
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <locale.h>

static _locale_t loc;
static int AF(_CRT_FLOAT *f, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atoflt_l(f, b, loc); }

TC_CASE(simple) { _CRT_FLOAT f; f.f = -1; CHECK_EQ_I(AF(&f, "2.5"), 0); CHECK_BITS_F(f.f, 2.5f); }
TC_CASE(negative) { _CRT_FLOAT f; f.f = 1; CHECK_EQ_I(AF(&f, "-1.25"), 0); CHECK_BITS_F(f.f, -1.25f); }

int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::setup _create_locale\n"); return 1; }
    TC_RUN(simple); TC_RUN(negative);
    _free_locale(loc);
    return tc_end();
}
