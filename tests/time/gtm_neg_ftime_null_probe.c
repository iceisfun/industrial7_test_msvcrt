/* TC
 * id:    gtm_neg_ftime_null_probe
 * area:  time
 * funcs: 
 * procs: _ftime
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ftime (NULL timeptr: parameter validation); _ftime64_s NULL -> EINVAL
 * note:  Probes: _ftime(NULL) invokes the invalid parameter handler exactly once, survives when a handler is installed, and sets errno EINVAL.
 *        The exact handler count/errno for the void-returning legacy _ftime is measured on real Windows.
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

#if TC_X64 || TC_CRT_VER >= 140
typedef struct __timeb64 TB;
#else
typedef struct __timeb32 TB;
#endif
typedef void (__cdecl *ftime_t)(TB *);
static ftime_t p_ftime;

TC_CASE(handler_called_once)
{
    if (!p_ftime) TC_SKIP("_ftime not exported");
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    p_ftime(NULL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(errno_einval)
{
    if (!p_ftime) TC_SKIP("_ftime not exported");
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    p_ftime(NULL);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin(); p_ftime = (ftime_t)crt_or("_ftime", "_ftime" TC_SZ);
    TC_RUN(handler_called_once); TC_RUN(errno_einval);
    return tc_end();
}
