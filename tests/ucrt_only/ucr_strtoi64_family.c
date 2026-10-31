/* TC
 * id:    ucr_strtoi64_family
 * area:  ucrt_only
 * funcs: _strtoi64 _strtoui64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strtoi64, _strtoi64_l, _strtoui64
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v;
TC_CASE(i64_limits) { char *e; v = "9223372036854775807"; errno = 0; CHECK_EQ_I(_strtoi64((const char *)v, &e, 10), 0x7fffffffffffffffLL); CHECK_EQ_I(errno, 0);
  v = "-9223372036854775808"; CHECK_EQ_U(_strtoi64((const char *)v, &e, 10), 0x8000000000000000ULL); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 20); }
TC_CASE(ui64_max) { char *e; v = "18446744073709551615"; errno = 0; CHECK_EQ_U(_strtoui64((const char *)v, &e, 10), 0xffffffffffffffffULL); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 20); }
TC_CASE(base0_prefix) { char *e; v = "0x1F"; CHECK_EQ_I(_strtoi64((const char *)v, &e, 0), 31); CHECK_EQ_P(e, v + 4);
  v = "017"; CHECK_EQ_I(_strtoi64((const char *)v, &e, 0), 15); v = "17"; CHECK_EQ_I(_strtoui64((const char *)v, &e, 0), 17); }
TC_CASE(trailing_garbage) { char *e; v = "12345abc"; CHECK_EQ_I(_strtoi64((const char *)v, &e, 10), 12345); CHECK_EQ_P(e, v + 5);
  v = "  -77 x"; CHECK_EQ_I(_strtoi64((const char *)v, &e, 10), -77); CHECK_EQ_P(e, v + 5); }
TC_CASE(beyond_32bit) { char *e; v = "4294967296"; CHECK_EQ_I(_strtoi64((const char *)v, &e, 10), 4294967296LL); CHECK_EQ_U(_strtoui64((const char *)v, &e, 16), 0x4294967296ULL); }

int main(void) { tc_begin(); TC_RUN(i64_limits); TC_RUN(ui64_max); TC_RUN(base0_prefix); TC_RUN(trailing_garbage); TC_RUN(beyond_32bit); return tc_end(); }
