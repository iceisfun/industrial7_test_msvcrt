/* TC
 * id:    pff_neg_printf_closed_stdout_child
 * area:  stdio_printf
 * kind:  negative
 * funcs: printf fprintf fflush
 * procs: _close
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fflush, _close; C11 7.21.5.2 (fflush returns EOF on write error)
 * note:  PROBES (in child processes): after _close(1), with stdout made fully buffered first, printf of a float buffers (or fails) and
 *        fflush(stdout) then returns EOF with errno EBADF; fprintf(stdout) + fflush: at least one of them fails. The process must
 *        not crash (the invalid-parameter handler is installed in the child, since _write on a closed fd may call it).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <io.h>

static int child_main(const char *n)
{
    int r, fr, bad = 0; volatile double v = 1.0; volatile double h = 0.5;
    tc_iph_install();
    if (setvbuf(stdout, NULL, _IOFBF, 4096) != 0) return 90;
    if (_close(1) != 0) return 91;
    errno = 0;
    if (n[0] == 'p') {
        r = printf("%f\n", v); (void)r;
        errno = 0;
        fr = fflush(stdout);
        if (fr != EOF) bad |= 1;
        if (errno != EBADF) bad |= 2;
    } else {
        r = fprintf(stdout, "%.2f\n", h);
        errno = 0;
        fr = fflush(stdout);
        if (r >= 0 && fr != EOF) bad |= 4;
    }
    return bad;
}
TC_CASE(printf_then_fflush)
{
    DWORD code = 0xff; int ran = tc_spawn_self("printf", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
TC_CASE(fprintf_stdout_then_fflush)
{
    DWORD code = 0xff; int ran = tc_spawn_self("fprintf", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(printf_then_fflush); TC_RUN(fprintf_stdout_then_fflush);
    return tc_end();
}
