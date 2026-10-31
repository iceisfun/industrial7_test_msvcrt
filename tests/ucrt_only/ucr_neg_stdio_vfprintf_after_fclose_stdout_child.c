/* TC
 * id:    ucr_neg_stdio_vfprintf_after_fclose_stdout_child
 * area:  ucrt_only
 * funcs: fclose
 * procs: __stdio_common_vfprintf __acrt_iob_func
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fclose, fprintf (invalid stream: negative return, errno EBADF/EINVAL)
 * note:  PROBES: using a FILE after fclose is undefined by ISO C and not defined by MS docs; the expectation (negative return,
 *        errno EBADF or EINVAL) is only likely. Child process closes stdout, then writes to it through __stdio_common_vfprintf. Exit code 77 means every check
 *        held; 1..4 identify the failing check. The child installs the invalid-parameter handler so it cannot be terminated.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
typedef FILE *(__cdecl *iob_t)(unsigned);
static vf_t p_vf; static iob_t p_iob;
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }

static int child_main(void)
{
    FILE *so; int r;
    if (!p_vf || !p_iob) return 1;
    so = p_iob(1);
    if (!so) return 1;
    fclose(so);
    errno = 0;
    r = callf(so, "text %d", 5);
    if (r >= 0) return 2;
    if (errno != EBADF && errno != EINVAL) return 3;
    return 77;
}
TC_CASE(child_checks)
{
    DWORD code = 0; int ran = tc_spawn_self("closed_stdout", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    p_iob = (iob_t)tc_crt_proc("__acrt_iob_func");
    if (tc_child_name()) return child_main();
    TC_RUN(child_checks);
    return tc_end();
}
