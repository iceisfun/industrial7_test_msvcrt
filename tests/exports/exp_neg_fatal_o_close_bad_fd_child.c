/* TC
 * id:    exp_neg_fatal_o_close_bad_fd_child
 * area:  exports
 * funcs:
 * procs: _o__close _o__set_invalid_parameter_handler _o__errno _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _close (EBADF, invalid parameter handler); _o_ aliases undocumented
 * note:  Windows-observed (Win10 22H2): _o__close runs _close under ucrtbase's separate OS global state (wrapped_invoke/enter_os_call). Its invalid-parameter
 *        handler and errno are the OS-state ones (_o__set_invalid_parameter_handler, _o__errno); the app-state handler/errno are not consulted.
 *        Child: with the OS-state handler restored to the UCRT default (NULL) _o__close(-1) is fatal (0xC0000409); restoring only the app-state handler is not enough (exit 77).
 *        Parent: OS-state handler installed -> -1, OS errno EBADF, one OS handler call; app state untouched.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *close_t)(int);
static close_t p_close;

static int o_iph_count;
static void __cdecl o_iph(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; o_iph_count++; }

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
    int r; int *(__cdecl *oerr)(void) = (int *(__cdecl *)(void))tc_crt_proc("_o__errno");
    void *(__cdecl *oset)(void *) = (void *(__cdecl *)(void *))tc_crt_proc("_o__set_invalid_parameter_handler");
    if (!p_close) TC_SKIP("_o__close not exported");
    if (!oerr || !oset || !tc_iph_install()) TC_SKIP("no handler api");
    oset((void *)o_iph);
    o_iph_count = 0; tc_iph_count = 0; errno = 0; *oerr() = 0;
    r = p_close(-1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(*oerr(), EBADF);
    CHECK_EQ_I(o_iph_count, 1);
    CHECK_EQ_I(errno, 0);          /* app state untouched */
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    p_close = (close_t)tc_crt_proc("_o__close");
    if ((c = tc_child_name())) {
        volatile int fd = -1;
        void (__cdecl *s)(void *) = (void (__cdecl *)(void *))tc_crt_proc("_set_invalid_parameter_handler");
        void (__cdecl *os)(void *) = (void (__cdecl *)(void *))tc_crt_proc("_o__set_invalid_parameter_handler");
        if (s) s(NULL);
        if (os) os(NULL);   /* the _o_ call consults the OS-state handler: restore the UCRT default (fatal) there too */
        if (p_close) p_close(fd);
        return 77;
    }
    TC_RUN(child_dies_on_bad_fd);
    TC_RUN(parent_handler_variant_returns_minus1_ebadf);
    return tc_end();
}
