/* TC
 * id:    ct_toupper_tolower_ascii_sweep
 * area:  ctype
 * funcs: toupper tolower
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.4.2.1, 7.4.2.2; MS docs toupper, tolower
 */
#include "tc.h"
#include <ctype.h>

static volatile int v;
#define TU(x) (v = (x), (toupper)(v))
#define TL(x) (v = (x), (tolower)(v))

TC_CASE(toupper_lower_to_upper) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(TU(c), (c >= 'a' && c <= 'z') ? c - 32 : c); }
TC_CASE(tolower_upper_to_lower) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(TL(c), (c >= 'A' && c <= 'Z') ? c + 32 : c); }
TC_CASE(roundtrip_letters) { int c; for (c = 'a'; c <= 'z'; c++) { CHECK_EQ_I(TL(TU(c)), c); CHECK_EQ_I(TU(TL(c - 32)), c - 32); } }
TC_CASE(toupper_tolower_idempotent) { int c; for (c = 0; c < 128; c++) { CHECK_EQ_I(TU(TU(c)), TU(c)); CHECK_EQ_I(TL(TL(c)), TL(c)); } }

int main(void) { tc_begin(); TC_RUN(toupper_lower_to_upper); TC_RUN(tolower_upper_to_lower); TC_RUN(roundtrip_letters); TC_RUN(toupper_tolower_idempotent); return tc_end(); }
