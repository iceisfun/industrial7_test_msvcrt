/* TC
 * id:    ucr_neg_strtol_overflow
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtol, strtoul (LONG is 32-bit on Windows)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const char *volatile v;
TC_CASE(pos_over) { char *e; long r; v = "2147483648"; errno = 0; r = strtol((const char *)v, &e, 10);
  CHECK_EQ_I(r, LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 10); }
TC_CASE(neg_over) { char *e; long r; v = "-2147483649"; errno = 0; r = strtol((const char *)v, &e, 10);
  CHECK_EQ_I(r, LONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 11); }
TC_CASE(ul_over) { char *e; unsigned long r; v = "4294967296"; errno = 0; r = strtoul((const char *)v, &e, 10);
  CHECK_EQ_U(r, ULONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 10); }
TC_CASE(endptr_after_digits) { char *e; long r; v = "99999999999xyz"; errno = 0; r = strtol((const char *)v, &e, 10);
  CHECK_EQ_I(r, LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 11); }
TC_CASE(boundary_ok) { char *e; long r; v = "2147483647"; errno = 0; r = strtol((const char *)v, &e, 10);
  CHECK_EQ_I(r, LONG_MAX); CHECK_EQ_I(errno, 0);
  v = "-2147483648"; r = strtol((const char *)v, &e, 10); CHECK_EQ_I(r, LONG_MIN); CHECK_EQ_I(errno, 0); }
TC_CASE(errno_not_cleared) { char *e; long r; v = "5"; errno = ERANGE; r = strtol((const char *)v, &e, 10);
  CHECK_EQ_I(r, 5); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(pos_over); TC_RUN(neg_over); TC_RUN(ul_over); TC_RUN(endptr_after_digits); TC_RUN(boundary_ok); TC_RUN(errno_not_cleared); return tc_end(); }
