/* TC
 * id:    ext_probe_neg_abort_no_stdio_flush
 * area:  exit_signal
 * funcs: abort fopen fputs fclose remove fflush
 * procs: _set_abort_behavior
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs abort (does not flush stream buffers - believed, wording not certain)
 * note:  PROBES that abort() leaves unflushed fopen()ed data unwritten (file size 0). Control case (fflush before abort) is separate.
 *        Win10 22H2 19045 (observed): after abort() (exit code 3) the buffered data is nevertheless in the file (size 14), same as _exit.
 *        [unverified] on other CRTs: only ucrtbase 19045 was observed; the expectation is gated with TC_AT_LEAST(140) and the original expectation (file stays empty) is kept for older CRTs.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>

#define FN_A "tc_ext_probe_abort_a.tmp"
#define FN_B "tc_ext_probe_abort_b.tmp"
typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
static const char text[] = "hello-buffered";

static long fsize(const char *fn)
{
    long n; FILE *f = fopen(fn, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END); n = ftell(f); fclose(f);
    return n;
}
TC_CASE(control_flushed)
{
    DWORD c = 0; remove(FN_B);
    CHECK(tc_spawn_self("flush", &c, 20000));
    CHECK_EQ_U(c, 3);
    CHECK_EQ_I(fsize(FN_B), (long)(sizeof text - 1));
    remove(FN_B);
}
TC_CASE(unflushed_lost)
{
    DWORD c = 0; remove(FN_A);
    CHECK(tc_spawn_self("noflush", &c, 20000));
    CHECK_EQ_U(c, 3);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(fsize(FN_A), (long)(sizeof text - 1));   /* observed on Win10 19045: abort() (exit code 3) still leaves the buffered data in the file */
#else
    CHECK_EQ_I(fsize(FN_A), 0);
#endif
    remove(FN_A);
}
int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        FILE *f;
        if (!sab) return 78;
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
        sab(0, 0x1 | 0x2);
        f = fopen(n[0] == 'n' ? FN_A : FN_B, "w");
        if (!f) return 79;
        fputs(text, f);
        if (n[0] == 'f') fflush(f);
        abort();
        return 77;
    }
    TC_RUN(control_flushed); TC_RUN(unflushed_lost);
    return tc_end();
}
