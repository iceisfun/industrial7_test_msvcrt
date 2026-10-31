/* TC
 * id:    ucr_neg_strtoi64_overflow
 * area:  ucrt_only
 * funcs: _strtoi64 _strtoui64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtoi64 (ERANGE, _I64_MAX/_I64_MIN/_UI64_MAX); invalid base -> EINVAL in msvcr80+
 * note:  bad-base case only for >=80: older CRTs have no invalid parameter handler.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v;
TC_CASE(too_big) { char *e; v = "9223372036854775808"; errno = 0; CHECK_EQ_I(_strtoi64((const char *)v, &e, 10), 0x7fffffffffffffffLL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 19); }
TC_CASE(too_small) { char *e; v = "-9223372036854775809"; errno = 0; CHECK_EQ_U(_strtoi64((const char *)v, &e, 10), 0x8000000000000000ULL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20); }
TC_CASE(unsigned_too_big) { char *e; v = "18446744073709551616"; errno = 0; CHECK_EQ_U(_strtoui64((const char *)v, &e, 10), 0xffffffffffffffffULL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20); }
TC_CASE(bad_base) {
#if TC_CRT_VER >= 80
  char *e; v = "10";
  if (!tc_iph_install()) TC_SKIP("no iph");
  tc_iph_count = 0; errno = 0; CHECK_EQ_I(_strtoi64((const char *)v, &e, 37), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
  tc_iph_count = 0; errno = 0; CHECK_EQ_U(_strtoui64((const char *)v, &e, 1), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
#else
  TC_SKIP("pre-msvcr80");
#endif
}

int main(void) { tc_begin(); TC_RUN(too_big); TC_RUN(too_small); TC_RUN(unsigned_too_big); TC_RUN(bad_base); return tc_end(); }
