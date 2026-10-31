/* TC
 * id:    ucr_neg_strtoll_overflow
 * area:  ucrt_only
 * funcs: strtoll strtoull
 * crt:   win10 msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtoll (invalid base -> EINVAL + invalid parameter handler)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const char *volatile v;
TC_CASE(pos_over) { char *e; v = "9223372036854775808"; errno = 0; CHECK_EQ_I(strtoll((const char *)v, &e, 10), LLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 19); }
TC_CASE(neg_over) { char *e; v = "-9223372036854775809"; errno = 0; CHECK_EQ_I(strtoll((const char *)v, &e, 10), LLONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20); }
TC_CASE(ull_over) { char *e; v = "18446744073709551616"; errno = 0; CHECK_EQ_U(strtoull((const char *)v, &e, 10), ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20); }
TC_CASE(bad_base) { char *e; long long r; v = "10";
  if (!tc_iph_install()) TC_SKIP("no iph");
  tc_iph_count = 0; errno = 0; r = strtoll((const char *)v, &e, 1);
  CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
  tc_iph_count = 0; errno = 0; r = (long long)strtoull((const char *)v, &e, 37);
  CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_RUN(pos_over); TC_RUN(neg_over); TC_RUN(ull_over); TC_RUN(bad_base); return tc_end(); }
