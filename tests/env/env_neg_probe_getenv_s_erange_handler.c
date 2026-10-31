/* TC
 * id:    env_neg_probe_getenv_s_erange_handler
 * area:  env
 * kind:  negative
 * procs: getenv_s
 * funcs: _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs getenv_s
 * note:  Probes (1) the invalid-parameter handler is NOT invoked for ERANGE (plain return) and (2) buffer[0] is set to NUL
 *        after ERANGE.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(int, getenv_s, (size_t *, char *, size_t, const char *));

TC_CASE(handler_not_invoked)
{
    size_t n = 99; char b[3] = "xy"; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    tc_iph_install(); tc_iph_count = 0;
    _putenv("TCV_GSP=hello");
    r = tc_p_getenv_s(&n, b, 3, "TCV_GSP");
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(tc_iph_count, 0);
    _putenv("TCV_GSP=");
}
TC_CASE(buffer0_nul)
{
    size_t n = 99; char b[3] = "xy"; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    tc_iph_install();
    _putenv("TCV_GSP=hello");
    r = tc_p_getenv_s(&n, b, 3, "TCV_GSP");
    CHECK_EQ_I(r, ERANGE);
    CHECK(b[0] == 0);
    _putenv("TCV_GSP=");
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(getenv_s);
    TC_RUN(handler_not_invoked); TC_RUN(buffer0_nul);
    return tc_end();
}
