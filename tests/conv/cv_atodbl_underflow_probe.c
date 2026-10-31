/* TC
 * id:    cv_atodbl_underflow_probe
 * area:  conv
 * funcs: _atodbl _atoflt
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _atodbl: _UNDERFLOW (_UNDERFLOW, 4 in math.h) returned when value underflows
 * note:  PROBING: that _atodbl("1e-999") / _atoflt("1e-99") return _UNDERFLOW and store 0;
 *        and that _atodbl("abc") (no conversion) returns 0 and stores +0.0 (not documented).
 */
#include "tc.h"
#include <stdlib.h>
#include <math.h>

TC_CASE(dbl_underflow) { _CRT_DOUBLE d; char b[] = "1e-999"; int r; d.x = -1; r = _atodbl(&d, b); CHECK_EQ_I(r, _UNDERFLOW); CHECK_BITS_D(d.x, 0.0); }
TC_CASE(flt_underflow) { _CRT_FLOAT f; char b[] = "1e-99"; int r; f.f = -1; r = _atoflt(&f, b); CHECK_EQ_I(r, _UNDERFLOW); CHECK_BITS_F(f.f, 0.0f); }
TC_CASE(garbage) { _CRT_DOUBLE d; char b[] = "abc"; int r; d.x = -1; r = _atodbl(&d, b); CHECK_EQ_I(r, 0); CHECK_BITS_D(d.x, 0.0); }

int main(void) { tc_begin(); TC_RUN(dbl_underflow); TC_RUN(flt_underflow); TC_RUN(garbage); return tc_end(); }
