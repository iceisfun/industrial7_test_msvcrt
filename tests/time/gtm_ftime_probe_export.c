/* TC
 * id:    gtm_ftime_probe_export
 * area:  time
 * funcs: 
 * procs: _ftime _ftime64
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ftime Requirements (_ftime is a documented function)
 * note:  Probes whether the plain name _ftime is a real DLL export in each CRT. Expected (from memory, unverified): present only in
 *        msvcrt.dll, absent from ucrtbase and msvcr80-120 (header inline). _ftime64 is the control (expected present everywhere listed).
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

/* control: the sized 64-bit form is documented for every listed CRT */
TC_CASE(ftime64_exported) { CHECK(tc_crt_proc("_ftime64") != NULL); }
/* probe: in the VS2005+ CRTs plain _ftime is a header inline for the sized forms, so (to the author's memory) it is NOT a DLL
 * export; legacy msvcrt.dll does export it. Expectation: exported only for msvcrt. */
TC_CASE(ftime_plain_export)
{
    int exported = tc_crt_proc("_ftime") != NULL;
    CHECK_EQ_I(exported, TC_CRT_VER == 60);
}
int main(void) { tc_begin(); TC_RUN(ftime64_exported); TC_RUN(ftime_plain_export); return tc_end(); }
