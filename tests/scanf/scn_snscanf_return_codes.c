/* TC
 * id:    scn_snscanf_return_codes
 * area:  scanf
 * funcs: _snscanf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf return value (number of fields converted; EOF if end of input before first conversion)
 */
#include "tc.h"
#include <stdio.h>

static volatile size_t cnt;

TC_CASE(exhausted_before_conv) { int v = 5, r; cnt = 2; r = _snscanf("  12", cnt, "%d", &v); CHECK_EQ_I(r, EOF); CHECK_EQ_I(v, 5); }
TC_CASE(mismatch) { int v = 5, r; cnt = 10; r = _snscanf("abc", cnt, "%d", &v); CHECK_EQ_I(r, 0); CHECK_EQ_I(v, 5); }
TC_CASE(partial) { int a = 0, b = 5, r; cnt = 10; r = _snscanf("1 x", cnt, "%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 5); }
TC_CASE(empty_string) { int v = 5, r; cnt = 10; r = _snscanf("", cnt, "%d", &v); CHECK_EQ_I(r, EOF); CHECK_EQ_I(v, 5); }
TC_CASE(full) { int a = 0, b = 0, r; cnt = 10; r = _snscanf("3 4", cnt, "%d %d", &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 3); CHECK_EQ_I(b, 4); }

int main(void) { tc_begin(); TC_RUN(exhausted_before_conv); TC_RUN(mismatch); TC_RUN(partial); TC_RUN(empty_string); TC_RUN(full); return tc_end(); }
