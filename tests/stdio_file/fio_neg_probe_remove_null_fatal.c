/* TC
 * id:    fio_neg_probe_remove_null_fatal
 * area:  stdio_file
 * funcs: remove
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs "Parameter validation"; remove
 * note:  Probes that remove(NULL) with the default invalid-parameter handler terminates the process abnormally on UCRT
 *        (the remove docs do not state a NULL check explicitly; older CRTs may just return -1 with errno set, so >=140 only).
 *        Runs in a child; the child returns 77 only if remove() returned.
 *        HARNESS FIX (class F, Win10 22H2): mingw crt2.o installs a no-op invalid-parameter handler at startup, so the child survived (exit 77 instead of
 *        0xC0000409); the child now calls tc_iph_default() (tc.h) to restore the CRT default before the call under test.
 *        OBSERVED (class A, Win10 22H2): remove(NULL) is NOT parameter-validated on ucrtbase; it returns -1/ENOENT even with the default handler, so the case now asserts the child survives.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(remove_null_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("rmnull", &code, 20000);
    CHECK(ran);
    /* observed Win10 22H2: with the CRT default handler restored, remove(NULL) still returns normally (no termination):
     * child exit 77 == remove returned -1 with errno ENOENT, 78 == returned something else. (raw: code != 77) */
    CHECK_EQ_U(code, 77);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) { const char *volatile p = NULL; int r; tc_iph_default(); errno = 0; r = remove(p); return (r == -1 && errno == ENOENT) ? 77 : 78; }
    TC_RUN(remove_null_returns);
    return tc_end();
}
