/* TC
 * id:    gtm_ftime_probe_tzdst
 * area:  time
 * funcs: 
 * procs: _ftime
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ftime (timezone = minutes west of UTC; dstflag nonzero if DST in effect)
 * note:  Probes that dstflag is exactly 0 or 1 (docs only say nonzero when DST) and that timezone is a sane minutes-west value.
 *        Kept apart from gtm_ftime_basic so one wrong guess cannot mask documented checks.
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


TC_CASE(tz_and_dst)
{
    TB t;
    if (!p_ftime) TC_SKIP("_ftime not exported");
    p_ftime(&t);
    CHECK(t.timezone >= -1440 && t.timezone <= 1440);
    CHECK(t.dstflag == 0 || t.dstflag == 1);
}
int main(void)
{
    tc_begin(); p_ftime = (ftime_t)crt_or("_ftime", "_ftime" TC_SZ);
    TC_RUN(tz_and_dst);
    return tc_end();
}
