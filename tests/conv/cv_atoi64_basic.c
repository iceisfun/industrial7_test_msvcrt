/* TC
 * id:    cv_atoi64_basic
 * area:  conv
 * funcs: _atoi64
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs atoi, _atoi64, _wtoi64
 */
#include "tc.h"
#include <stdlib.h>

static __int64 A(const char *volatile s) { return _atoi64(s); }

TC_CASE(max) { CHECK_EQ_I(A("9223372036854775807"), 0x7fffffffffffffffLL); }
TC_CASE(min) { CHECK_EQ_I(A("-9223372036854775808"), -0x7fffffffffffffffLL - 1); }
TC_CASE(ws_sign) { CHECK_EQ_I(A("   +55"), 55LL); CHECK_EQ_I(A("\t\n-66"), -66LL); CHECK_EQ_I(A("\v\f\r77"), 77LL); }
TC_CASE(garbage_tail) { CHECK_EQ_I(A("4294967296zz"), 4294967296LL); CHECK_EQ_I(A("-1.5"), -1LL); }
TC_CASE(empty) { CHECK_EQ_I(A(""), 0LL); CHECK_EQ_I(A("  "), 0LL); CHECK_EQ_I(A("abc"), 0LL); }
TC_CASE(hex_prefix) { CHECK_EQ_I(A("0x1F"), 0LL); }

int main(void) { tc_begin(); TC_RUN(max); TC_RUN(min); TC_RUN(ws_sign); TC_RUN(garbage_tail); TC_RUN(empty); TC_RUN(hex_prefix); return tc_end(); }
