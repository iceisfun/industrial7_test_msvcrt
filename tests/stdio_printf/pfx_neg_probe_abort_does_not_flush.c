/* TC
 * id:    pfx_neg_probe_abort_does_not_flush
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen abort
 * procs: _set_abort_behavior
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs abort ("exit code 3", does not flush stream buffers / no atexit processing)
 * note:  Probes that abort() (with message box and Watson suppressed) ends the child with code 3 and that the buffered
 *        4 bytes never reach the file. Docs say abort does not flush buffers; the exact exit code 3 is the probed part.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>

#define FN "tc_pfx_neg_probe_abort_does_not_flush.tmp"

TC_DECL_PROC(unsigned int, _set_abort_behavior, (unsigned int, unsigned int));

static long long fsize(const char *n)
{
    WIN32_FILE_ATTRIBUTE_DATA d;
    if (!GetFileAttributesExA(n, GetFileExInfoStandard, &d)) return -1;
    return ((long long)d.nFileSizeHigh << 32) | d.nFileSizeLow;
}

TC_CASE(abort_code)
{
    DWORD code = 0; int ran;
    DeleteFileA(FN);
    ran = tc_spawn_self("abort", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 3);
}
TC_CASE(data_discarded)
{
    CHECK_EQ_I(fsize(FN), 0);
    DeleteFileA(FN);
}

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        FILE *f;
        TC_LOAD_PROC(_set_abort_behavior);
        if (tc_p__set_abort_behavior) tc_p__set_abort_behavior(0, 0x1 | 0x2); /* _WRITE_ABORT_MSG | _CALL_REPORTFAULT */
        f = fopen(FN, "w");
        if (!f) return 70;
        if (fprintf(f, "data") != 4) return 71;
        abort();
        return 77;
    }
    TC_RUN(abort_code); TC_RUN(data_discarded);
    return tc_end();
}
