/* TC
 * id:    err_strerror_keeps_errno
 * area:  errno_param
 * funcs: strerror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.5; MS docs strerror
 * note:  PROBE: strerror leaves errno untouched (known and unknown errnum).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile int v_e;

TC_CASE(known)   { errno = ERANGE; v_e = ENOENT; (void)strerror(v_e); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(unknown) { errno = ERANGE; v_e = 9999;   (void)strerror(v_e); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(known); TC_RUN(unknown); return tc_end(); }
