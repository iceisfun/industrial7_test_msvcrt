/* TC
 * id:    err_const_posix_supplement_values
 * area:  errno_param
 * funcs: _errno
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs errno constants (POSIX supplement)
 * note:  Header-only; checks values match the SDK the corpus is built with.
 */
#include "tc.h"
#include <errno.h>

TC_CASE(a) { CHECK_EQ_I(EADDRINUSE, 100); CHECK_EQ_I(EADDRNOTAVAIL, 101); CHECK_EQ_I(EAFNOSUPPORT, 102); CHECK_EQ_I(EALREADY, 103); CHECK_EQ_I(EBADMSG, 104); CHECK_EQ_I(ECANCELED, 105); }
TC_CASE(b) { CHECK_EQ_I(ECONNABORTED, 106); CHECK_EQ_I(ECONNREFUSED, 107); CHECK_EQ_I(ECONNRESET, 108); CHECK_EQ_I(EHOSTUNREACH, 110); CHECK_EQ_I(EINPROGRESS, 112); CHECK_EQ_I(ELOOP, 114); }
TC_CASE(c) { CHECK_EQ_I(ENOTSUP, 129); CHECK_EQ_I(EOPNOTSUPP, 130); CHECK_EQ_I(EOVERFLOW, 132); CHECK_EQ_I(EPROTO, 134); CHECK_EQ_I(ETIMEDOUT, 138); CHECK_EQ_I(EWOULDBLOCK, 140); }
int main(void) { tc_begin(); (void)_errno(); TC_RUN(a); TC_RUN(b); TC_RUN(c); return tc_end(); }
