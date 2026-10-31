/* TC
 * id:    cv_atodbl_atoflt
 * area:  conv
 * funcs: _atodbl _atoflt
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _atodbl, _atoflt (return 0, _UNDERFLOW or _OVERFLOW from <math.h>/<stdlib.h> _CRT_DOUBLE/_CRT_FLOAT)
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>

static int AD(_CRT_DOUBLE *d, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atodbl(d, b); }
static int AF(_CRT_FLOAT *f, const char *s) { char b[64]; int i = 0; while (s[i]) { b[i] = s[i]; i++; } b[i] = 0; return _atoflt(f, b); }

TC_CASE(dbl) { _CRT_DOUBLE d; int r; d.x = -1; r = AD(&d, "1.5"); CHECK_EQ_I(r, 0); CHECK_BITS_D(d.x, 1.5); }
TC_CASE(flt) { _CRT_FLOAT f; int r; f.f = -1; r = AF(&f, "2.5"); CHECK_EQ_I(r, 0); CHECK_BITS_F(f.f, 2.5f); }
TC_CASE(overflow) { _CRT_DOUBLE d; _CRT_FLOAT f; CHECK_EQ_I(AD(&d, "1e999"), _OVERFLOW); CHECK_EQ_I(AF(&f, "1e99"), _OVERFLOW); }

int main(void) { tc_begin(); TC_RUN(dbl); TC_RUN(flt); TC_RUN(overflow); return tc_end(); }
