/* TC
 * id:    ucr_neg_stdio_vfprintf_unbuffered_stdin_write_probe
 * area:  ucrt_only
 * funcs: freopen fclose remove fopen fputs
 * procs: __stdio_common_vfprintf __acrt_iob_func
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fprintf / freopen
 * note:  PROBES: writing with vfprintf to the stdin stream (__acrt_iob_func(0)) after freopen of stdin from a file ("r").
 *        Child exit code: 77 = ret<0 with errno EBADF, 78 = ret>=0 (accepted), others = unexpected errno.
 *        temp file tc_ucr_neg_stdio_vfprintf_unbuffered_stdin_write_probe_a.tmp (created by the parent).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
typedef FILE *(__cdecl *iob_t)(unsigned);
static vf_t p_vf; static iob_t p_iob;
#define TMP "tc_ucr_neg_stdio_vfprintf_unbuffered_stdin_write_probe_a.tmp"
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }

static int child_main(void)
{
    FILE *si; int r;
    if (!p_vf || !p_iob) return 1;
    si = p_iob(0);
    if (!si || !freopen(TMP, "r", si)) return 2;
    errno = 0;
    r = callf(si, "x%d", 1);
    if (r < 0) return errno == EBADF ? 77 : 3;
    return 78;
}
TC_CASE(child)
{
    DWORD code = 0; int ran; FILE *f = fopen(TMP, "wb");
    CHECK(f != NULL); if (!f) return;
    fputs("data", f); fclose(f);
    ran = tc_spawn_self("stdin_write", &code, 20000);
    CHECK(ran);
    CHECK(code == 77 || code == 78);
    remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    p_iob = (iob_t)tc_crt_proc("__acrt_iob_func");
    if (tc_child_name()) return child_main();
    TC_RUN(child);
    return tc_end();
}
