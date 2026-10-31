/* TC
 * id:    nfx_neg_close_minus2_no_handler
 * area:  negative_fatal
 * kind:  negative
 * funcs: _close _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _close (returns -1 and sets errno EBADF for a bad descriptor)
 * note:  Probes that _close(-2) (the "no console" sentinel) returns -1/EBADF WITHOUT invoking the invalid-parameter handler,
 *        whereas _close(-1) does invoke it. Split out of nfx_neg_iph_default_ucrt_close because it is recalled behaviour, not documented.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
static volatile int v_fd;
TC_CASE(minus2_no_handler)
{
    int r;
    tc_iph_count = 0;
    CHECK(tc_iph_install());
    v_fd = -2; errno = 0; r = _close(v_fd);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 0);
    _set_invalid_parameter_handler(NULL);
}
TC_CASE(minus1_handler)
{
    int r;
    tc_iph_count = 0;
    CHECK(tc_iph_install());
    v_fd = -1; errno = 0; r = _close(v_fd);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
    _set_invalid_parameter_handler(NULL);
}
int main(void) { tc_begin(); TC_RUN(minus2_no_handler); TC_RUN(minus1_handler); return tc_end(); }
