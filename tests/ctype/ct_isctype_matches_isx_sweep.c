/* TC
 * id:    ct_isctype_matches_isx_sweep
 * area:  ctype
 * funcs: _isctype isalpha isdigit ispunct isupper islower iscntrl isxdigit
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _isctype, isalpha
 */
#include "tc.h"
#include <ctype.h>

TC_CASE(alpha) { volatile int i; for (i = -1; i <= 255; i++) CHECK_EQ_I((_isctype(i, _ALPHA) != 0), ((isalpha)(i) != 0)); }
TC_CASE(digit) { volatile int i; for (i = -1; i <= 255; i++) CHECK_EQ_I((_isctype(i, _DIGIT) != 0), ((isdigit)(i) != 0)); }
TC_CASE(space) { volatile int i; for (i = -1; i <= 255; i++) CHECK_EQ_I((_isctype(i, _SPACE) != 0), ((isspace)(i) != 0)); }
TC_CASE(punct) { volatile int i; for (i = -1; i <= 255; i++) CHECK_EQ_I((_isctype(i, _PUNCT) != 0), ((ispunct)(i) != 0)); }
TC_CASE(upper) { volatile int i; for (i = -1; i <= 255; i++) CHECK_EQ_I((_isctype(i, _UPPER) != 0), ((isupper)(i) != 0)); }
TC_CASE(lower) { volatile int i; for (i = -1; i <= 255; i++) CHECK_EQ_I((_isctype(i, _LOWER) != 0), ((islower)(i) != 0)); }
TC_CASE(cntrl) { volatile int i; for (i = -1; i <= 255; i++) CHECK_EQ_I((_isctype(i, _CONTROL) != 0), ((iscntrl)(i) != 0)); }
TC_CASE(xdigit) { volatile int i; for (i = -1; i <= 255; i++) CHECK_EQ_I((_isctype(i, _HEX) != 0), ((isxdigit)(i) != 0)); }

int main(void) { tc_begin(); TC_RUN(alpha); TC_RUN(digit); TC_RUN(space); TC_RUN(punct); TC_RUN(upper); TC_RUN(lower); TC_RUN(cntrl); TC_RUN(xdigit); return tc_end(); }
