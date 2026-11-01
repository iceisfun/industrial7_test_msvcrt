/* TC
 * id:    gtm_neg_default_handler_fatal_ftime
 * area:  time
 * funcs: 
 * procs: _ftime _ftime64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs 'Parameter validation'; _ftime (NULL timeptr invokes the invalid parameter handler)
 * note:  Probes that _ftime(NULL) reaches the invalid-parameter handler and that the UCRT default handler terminates with fast-fail 0xC0000409.
 *        The child uses _ftime, falling back to _ftime64 if the plain name is not exported. Child exit code 77 means the call returned.
 */
#include "tc.h"
#include <sys/utime.h>
#include <sys/timeb.h>
#include <time.h>
#include <errno.h>
#include <io.h>
#include <wchar.h>
#include <fcntl.h>
#include <sys/stat.h>
#if TC_X64 || TC_CRT_VER >= 140
#define TC_SZ "64"
#else
#define TC_SZ "32"
#endif
/* plain name if exported, else the sized equivalent (same semantics where time_t is 64-bit / x86 32-bit) */
static FARPROC crt_or(const char *a, const char *b) { FARPROC p = tc_crt_proc(a); return p ? p : tc_crt_proc(b); }

typedef void (__cdecl *fn_t)(void *);
TC_CASE(null_timeptr)
{
    DWORD code = 0; int ran = tc_spawn_self("nullftime", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(parent_alive) { CHECK(1); }
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        tc_iph_default();  /* mingw startup installs a no-op handler; restore the CRT default (fast-fail) */
        fn_t f = (fn_t)crt_or("_ftime", "_ftime" TC_SZ);
        if (!f) return 78;
        f(NULL);
        return 77;
    }
    TC_RUN(null_timeptr); TC_RUN(parent_alive);
    return tc_end();
}
