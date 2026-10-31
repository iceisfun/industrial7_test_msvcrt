/* TC
 * id:    m99_nan_basic
 * area:  math_c99
 * funcs: nan nanf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.11.2; MS docs nan, nanf, nanl
 * note:  Only NaN-ness and the quiet bit are asserted; sign and payload are implementation-defined (sign checked in m99_nan_payload_probe).
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

TC_CASE(empty) { volatile const char *t = ""; CHECK_IS_NAN(nan((const char *)t)); }
TC_CASE(zero_tag) { volatile const char *t = "0"; CHECK_IS_NAN(nan((const char *)t)); }
TC_CASE(float_empty) { volatile const char *t = ""; float r = nanf((const char *)t); CHECK(r != r); }
TC_CASE(quiet) { volatile const char *t = ""; unsigned long long b = tc_dbits(nan((const char *)t));
    CHECK_EQ_U((b >> 52) & 0x7ff, 0x7ff); CHECK_EQ_U((b >> 51) & 1, 1); }
TC_CASE(float_quiet) { volatile const char *t = ""; unsigned int b = tc_fbits(nanf((const char *)t));
    CHECK_EQ_U((b >> 23) & 0xff, 0xff); CHECK_EQ_U((b >> 22) & 1, 1); }

int main(void) { tc_begin(); TC_RUN(empty); TC_RUN(zero_tag); TC_RUN(float_empty); TC_RUN(quiet); TC_RUN(float_quiet); return tc_end(); }
