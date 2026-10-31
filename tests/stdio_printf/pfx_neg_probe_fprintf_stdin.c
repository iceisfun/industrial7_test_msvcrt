/* TC
 * id:    pfx_neg_probe_fprintf_stdin
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fprintf (negative on error); errno EBADF for stream not open for writing
 * note:  PROBES fprintf(stdin, ...) (a stream not open for output): expected -1, errno EBADF, and no invalid-parameter handler call.
 *        Runs in a child so stdin state is not disturbed. Child exit code = 0x40 | 1 (r<0) | 2 (errno==EBADF) | 4 (no iph call).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static int child_main(void)
{
    int r, e, code = 0x40;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = fprintf(stdin, "%s", "x"); e = errno;
    if (r < 0) code |= 1;
    if (e == EBADF) code |= 2;
    if (tc_iph_count == 0) code |= 4;
    return code;
}
TC_CASE(child)
{
    DWORD code = 0; int ran = tc_spawn_self("stdin", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code & 0xF0, 0x40);
    CHECK(code & 1);
    CHECK(code & 2);
    CHECK(code & 4);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(child);
    return tc_end();
}
