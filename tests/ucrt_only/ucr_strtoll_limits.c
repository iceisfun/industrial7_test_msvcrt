/* TC
 * id:    ucr_strtoll_limits
 * area:  ucrt_only
 * funcs: strtoll strtoull
 * crt:   win10 msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtoll, strtoull
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const char *volatile v;
TC_CASE(llong_limits) { char *e; v = "9223372036854775807"; errno = 0; CHECK_EQ_I(strtoll((const char *)v, &e, 10), LLONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 19);
  v = "-9223372036854775808"; CHECK_EQ_I(strtoll((const char *)v, &e, 10), LLONG_MIN); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 20); }
TC_CASE(ullong_max) { char *e; v = "18446744073709551615"; errno = 0; CHECK_EQ_U(strtoull((const char *)v, &e, 10), ULLONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 20); }
TC_CASE(hex_all_ones) { char *e; v = "0xFFFFFFFFFFFFFFFF"; errno = 0; CHECK_EQ_U(strtoull((const char *)v, &e, 16), ULLONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 18);
  CHECK_EQ_U(strtoull((const char *)v, &e, 0), ULLONG_MAX); }
TC_CASE(base36) { char *e; v = "zz"; CHECK_EQ_I(strtoll((const char *)v, &e, 36), 35 * 36 + 35); CHECK_EQ_P(e, v + 2);
  v = "ZZ"; CHECK_EQ_I(strtoull((const char *)v, &e, 36), 1295); }
TC_CASE(ull_negative) { char *e; v = "-1"; CHECK_EQ_U(strtoull((const char *)v, &e, 10), ULLONG_MAX); }

int main(void) { tc_begin(); TC_RUN(llong_limits); TC_RUN(ullong_max); TC_RUN(hex_all_ones); TC_RUN(base36); TC_RUN(ull_negative); return tc_end(); }
