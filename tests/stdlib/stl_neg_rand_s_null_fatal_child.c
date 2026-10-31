/* TC
 * id:    stl_neg_rand_s_null_fatal_child
 * area:  stdlib
 * kind:  negative
 * funcs: rand_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation", rand_s
 * note:  rand_s(NULL) with the default invalid-parameter handler must terminate the process abnormally (documented: only
 *        "terminates"; exit code != 0 and != 77 is asserted). The exact status (0xC0000409 on ucrt) is probed in its own case.
 *        The second child installs a returning handler: expects EINVAL and the process continues (child exits 40).
 */
#define _CRT_RAND_S
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(default_handler_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nohandler", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(default_handler_exact_status_probe)
{
    DWORD code = 0; int ran = tc_spawn_self("nohandler", &code, 20000);
    CHECK(ran);
#ifdef TC_UCRT
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
#else
    CHECK(code == TC_STATUS_FASTFAIL || code == TC_STATUS_INVALID_CRT_PARAM);
#endif
}
TC_CASE(handler_returns_child_continues)
{
    DWORD code = 0; int ran = tc_spawn_self("withhandler", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 40);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile unsigned int *volatile p = NULL;
        if (c[0] == 'n') { rand_s((unsigned int *)p); return 77; }
        tc_iph_install(); tc_iph_count = 0; errno = 0;
        { errno_t r = rand_s((unsigned int *)p); return (r == EINVAL && errno == EINVAL && tc_iph_count == 1) ? 40 : 41; }
    }
    TC_RUN(default_handler_terminates); TC_RUN(default_handler_exact_status_probe); TC_RUN(handler_returns_child_continues);
    return tc_end();
}
