/* TC
 * id:    m99_nan_neg_garbage_tag
 * area:  math_c99
 * funcs: nan nanf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.11.2
 * note:  Any tag still yields a NaN; payload unspecified.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

TC_CASE(words) { volatile const char *t = "not a number"; tc_iph_install(); tc_iph_count = 0; CHECK_IS_NAN(nan((const char *)t)); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(overlong) { volatile const char *t = "99999999999999999999999"; CHECK_IS_NAN(nan((const char *)t)); }
TC_CASE(minus1) { volatile const char *t = "-1"; CHECK_IS_NAN(nan((const char *)t)); }
TC_CASE(float_long) { volatile const char *t = "999999999999999999999999999999 xyz"; float r = nanf((const char *)t); CHECK(r != r); }

int main(void) { tc_begin(); TC_RUN(words); TC_RUN(overlong); TC_RUN(minus1); TC_RUN(float_long); return tc_end(); }
