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
 * note:  abort() (message box and Watson suppressed) ends the child with exit code 3. OBSERVED Win10 22H2 19045 ucrtbase (run twice): the 4 buffered bytes ARE in the file
 *        after abort() (size 4) although the file was still empty just before the call (the child checks that: size 0 before abort, else exit 72, also with a full-buffered stream
 *        in a scratch probe). So this UCRT flushes open streams on abort; the old expectation "never reach the file" (from the docs wording) was wrong here.
 *        Deviates from the MS docs wording; C11 7.22.4.1 leaves it implementation-defined whether open output streams are flushed. class B.
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
TC_CASE(data_flushed_by_abort)
{
    CHECK_EQ_I(fsize(FN), 4);
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
        { WIN32_FILE_ATTRIBUTE_DATA d; if (!GetFileAttributesExA(FN, GetFileExInfoStandard, &d) || d.nFileSizeLow != 0) return 72; } /* still buffered before abort */
        abort();
        return 77;
    }
    TC_RUN(abort_code); TC_RUN(data_flushed_by_abort);
    return tc_end();
}
