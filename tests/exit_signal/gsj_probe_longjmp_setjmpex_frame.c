/* TC
 * id:    gsj_probe_longjmp_setjmpex_frame
 * area:  exit_signal
 * funcs: longjmp
 * procs: _setjmpex _setjmp
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _setjmp (x64 CRT export takes a frame pointer as second argument; unofficial)
 * note:  Probes that the x64 two-argument export form _setjmp(buf, frame)/_setjmpex(buf, frame) is accepted and that
 *        longjmp resumes at its call site. ucrtbase lacks these exports (vcruntime), so it is excluded.
 */
#include "tc.h"
#include <setjmp.h>

typedef int (__cdecl *sj_t)(void *, void *);
static sj_t p_setjmp, p_setjmpex;
static jmp_buf jb;

/* calling the raw export from a function that stays live across longjmp */
__attribute__((noinline)) static int try_sj(sj_t f, int *after)
{
    int r = f(jb, __builtin_frame_address(0));
    if (r == 0) { *after = 1; longjmp(jb, 7); }
    return r;
}
TC_CASE(setjmp_export)
{
    int after = 0, r;
    if (!p_setjmp) TC_SKIP("_setjmp not exported");
    r = try_sj(p_setjmp, &after);
    CHECK_EQ_I(r, 7); CHECK_EQ_I(after, 1);
}
TC_CASE(setjmpex_export)
{
    int after = 0, r;
    if (!p_setjmpex) TC_SKIP("_setjmpex not exported");
    r = try_sj(p_setjmpex, &after);
    CHECK_EQ_I(r, 7); CHECK_EQ_I(after, 1);
}
int main(void)
{
    tc_begin();
    p_setjmp = (sj_t)tc_crt_proc("_setjmp"); p_setjmpex = (sj_t)tc_crt_proc("_setjmpex");
    TC_RUN(setjmp_export); TC_RUN(setjmpex_export);
    return tc_end();
}
