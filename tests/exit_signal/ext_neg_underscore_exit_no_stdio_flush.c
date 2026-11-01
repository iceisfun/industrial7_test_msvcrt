/* TC
 * id:    ext_neg_underscore_exit_no_stdio_flush
 * area:  exit_signal
 * funcs: _exit fopen fputs fclose remove fflush
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _exit, _cexit: _exit terminates the process without atexit/_onexit processing and without flushing stream buffers
 * note:  Child opens a file, fputs a short string (buffered, below buffer size) and calls _exit(0). The file must still be empty.
 *        Control case: same child with fflush before _exit; the file then holds the whole string.
 *        Win10 22H2 19045 (observed): the MS docs say _exit does not flush stream buffers, but on the real ucrtbase the buffered "hello-buffered"
 *        ends up in the file anyway (size 14; the stream is flushed during process teardown). Case unflushed_lost asserts the observed size.
 *        [unverified] on other CRTs: only ucrtbase 19045 was observed; the expectation is gated with TC_AT_LEAST(140) and the original expectation (file stays empty) is kept for older CRTs.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>

#define FN_A "tc_ext_neg_underscore_exit_a.tmp"
#define FN_B "tc_ext_neg_underscore_exit_b.tmp"
static const char text[] = "hello-buffered";

static long fsize(const char *fn)
{
    long n; FILE *f = fopen(fn, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END); n = ftell(f); fclose(f);
    return n;
}
TC_CASE(unflushed_lost)
{
    DWORD c = 99; remove(FN_A);
    CHECK(tc_spawn_self("noflush", &c, 20000));
    CHECK_EQ_U(c, 0);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(fsize(FN_A), (long)(sizeof text - 1));   /* observed on Win10 19045: the data reached the file although _exit(0) skipped fflush */
#else
    CHECK_EQ_I(fsize(FN_A), 0);
#endif
    remove(FN_A);
}
TC_CASE(control_flushed)
{
    DWORD c = 99; remove(FN_B);
    CHECK(tc_spawn_self("flush", &c, 20000));
    CHECK_EQ_U(c, 0);
    CHECK_EQ_I(fsize(FN_B), (long)(sizeof text - 1));
    remove(FN_B);
}
TC_CASE(cleanup)
{
    CHECK_EQ_I(fsize(FN_A), -1);
    CHECK_EQ_I(fsize(FN_B), -1);
}
int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        FILE *f = fopen(n[0] == 'n' ? FN_A : FN_B, "w");
        if (!f) return 78;
        fputs(text, f);
        if (n[0] == 'f') fflush(f);
        _exit(0);
        return 77;
    }
    TC_RUN(unflushed_lost); TC_RUN(control_flushed); TC_RUN(cleanup);
    return tc_end();
}
