/* TC
 * id:    exp_neg_fatal_o_close_bad_fd_child
 * area:  exports
 * funcs:
 * procs: _o__close _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _close (EBADF, invalid parameter handler); _o_ aliases undocumented
 * note:  kind=negative conf=probe. Probing: _o__close(-1) behaves like _close(-1): fatal by default
 *        (child dies with a fast-fail status, not a normal exit); with a handler installed returns -1/EBADF.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *close_t)(int);
static close_t p_close;

TC_CASE(child_dies_on_bad_fd)
{
    DWORD code = 0; int ran;
    if (!p_close) TC_SKIP("_o__close not exported");
    ran = tc_spawn_self("close", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(parent_handler_variant_returns_minus1_ebadf)
{
    int r;
    if (!p_close) TC_SKIP("_o__close not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    r = p_close(-1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    const char *c;
    tc_begin();
    p_close = (close_t)tc_crt_proc("_o__close");
    if ((c = tc_child_name())) { volatile int fd = -1; if (p_close) p_close(fd); return 77; }
    TC_RUN(child_dies_on_bad_fd);
    TC_RUN(parent_handler_variant_returns_minus1_ebadf);
    return tc_end();
}
