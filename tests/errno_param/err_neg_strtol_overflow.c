/* TC
 * id:    err_neg_strtol_overflow
 * area:  errno_param
 * kind:  negative
 * funcs: strtol strtoul
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtol, strtoul
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

static const char *volatile v_pos = "99999999999999999999";
static const char *volatile v_neg = "-99999999999999999999";

TC_CASE(pos) { char *e; long r; errno = 0; r = strtol(v_pos, &e, 10); CHECK_EQ_I(r, LONG_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg) { char *e; long r; errno = 0; r = strtol(v_neg, &e, 10); CHECK_EQ_I(r, LONG_MIN); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(unsigned_ovf) { char *e; unsigned long r; errno = 0; r = strtoul(v_pos, &e, 10); CHECK_EQ_U(r, ULONG_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(endptr) { char *e = NULL; const char *s = v_pos; strtol(s, &e, 10); CHECK_EQ_P(e, s + 20); e = NULL; s = "7fffffffffffffffffff!"; strtoul(s, &e, 16); CHECK_EQ_P(e, s + 20); }

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(unsigned_ovf); TC_RUN(endptr); return tc_end(); }
