/* TC
 * id:    cv_strtol_no_digits_errno_probe
 * area:  conv
 * funcs: strtol
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.4 (leaves errno unspecified for no conversion; implementations may set EINVAL)
 * note:  PROBE: strtol("zzz", base 10) with no conversion leaves errno unchanged (0). Believed true for Microsoft CRTs.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile p;

TC_CASE(errno_untouched) { char *e; errno = 0; p = "zzz"; CHECK_EQ_I(strtol(p, &e, 10), 0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(errno_untouched); return tc_end(); }
