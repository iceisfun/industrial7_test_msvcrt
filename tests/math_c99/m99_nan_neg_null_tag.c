/* TC
 * id:    m99_nan_neg_null_tag
 * area:  math_c99
 * funcs: nan nanf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs nan (NULL tagp not documented)
 * note:  PROBE: nan(NULL)/nanf(NULL) must not crash; either returns NaN or calls the invalid-parameter handler (ucrt only; count 0 or 1).
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
#define DINF (v_one / v_zero)
#define FINF (f_one / f_zero)

TC_CASE(null_double) { volatile const char *t = NULL; double r; tc_iph_install(); tc_iph_count = 0; r = nan((const char *)t);
    CHECK(tc_iph_count == 0 || tc_iph_count == 1); CHECK(r != r); }
TC_CASE(null_float) { volatile const char *t = NULL; float r; tc_iph_install(); tc_iph_count = 0; r = nanf((const char *)t);
    CHECK(tc_iph_count == 0 || tc_iph_count == 1); CHECK(r != r); }

int main(void) { tc_begin(); TC_RUN(null_double); TC_RUN(null_float); return tc_end(); }
