/* TC
 * id:    ucr_neg_crt_atexit_null_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _crt_atexit
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter validation"; _crt_atexit is an undocumented ucrt export
 * note:  PROBES whether _crt_atexit(NULL) is rejected. The UCRT validates the table pointer but (to our recollection) not the
 *        function pointer, so a NULL handler is accepted silently and skipped at exit: expected return 0, errno untouched,
 *        invalid-parameter handler NOT called. Child encodes: bit0 = ret!=0, bit1 = errno==EINVAL, bit2 = iph called; expected 0.
 *        If real Windows reports 7 instead, NULL is rejected as a parameter error.
 *        Runs in a child and leaves via _exit() so that a (hypothetically) accepted NULL handler is never invoked.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef int (__cdecl *atexit_t)(void (__cdecl *)(void));

TC_CASE(null_handler)
{
    DWORD code = 0; int ran = tc_spawn_self("nullreg", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        atexit_t f = (atexit_t)tc_crt_proc("_crt_atexit");
        int r, code = 0;
        if (!f) _exit(90);
        tc_iph_install(); errno = 0;
        r = f((void (__cdecl *)(void))(volatile void *)0);
        if (r != 0) code |= 1;
        if (errno == EINVAL) code |= 2;
        if (tc_iph_count) code |= 4;
        _exit(code);
    }
    TC_RUN(null_handler);
    return tc_end();
}
