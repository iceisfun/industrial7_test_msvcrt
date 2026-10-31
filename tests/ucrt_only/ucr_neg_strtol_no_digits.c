/* TC
 * id:    ucr_neg_strtol_no_digits
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtol
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v;
TC_CASE(letters) { char *e; v = "abc"; CHECK_EQ_I(strtol((const char *)v, &e, 10), 0); CHECK_EQ_P(e, v);
  CHECK_EQ_U(strtoul((const char *)v, &e, 10), 0); CHECK_EQ_P(e, v); }
TC_CASE(empty) { char *e; v = ""; CHECK_EQ_I(strtol((const char *)v, &e, 10), 0); CHECK_EQ_P(e, v);
  CHECK_EQ_U(strtoul((const char *)v, &e, 10), 0); CHECK_EQ_P(e, v); }
TC_CASE(sign_only) { char *e; v = "  -"; CHECK_EQ_I(strtol((const char *)v, &e, 10), 0); CHECK_EQ_P(e, v);
  v = "+"; CHECK_EQ_U(strtoul((const char *)v, &e, 10), 0); CHECK_EQ_P(e, v); }
TC_CASE(hex_prefix_no_digits) { char *e; v = "0xg"; CHECK_EQ_I(strtol((const char *)v, &e, 16), 0); CHECK_EQ_U(strtoul((const char *)v, &e, 16), 0); }
TC_CASE(digit_out_of_base) { char *e; v = "9"; CHECK_EQ_I(strtol((const char *)v, &e, 8), 0); CHECK_EQ_P(e, v); }

int main(void) { tc_begin(); TC_RUN(letters); TC_RUN(empty); TC_RUN(sign_only); TC_RUN(hex_prefix_no_digits); TC_RUN(digit_out_of_base); return tc_end(); }
