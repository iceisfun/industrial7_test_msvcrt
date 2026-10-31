/* TC
 * id:    stl_neg_set_error_mode_errno_probe
 * area:  stdlib
 * kind:  negative
 * funcs: _set_error_mode
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_error_mode
 * note:  Probes that ucrtbase sets errno to EINVAL (via its parameter validation) for an invalid mode.
 *        Older CRTs are not covered because they may return -1 without touching errno.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int m_4 = 4;
TC_CASE(einval) { errno = 0; CHECK_EQ_I(_set_error_mode(m_4), -1); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(einval); return tc_end(); }
