/* TC
 * id:    gsf_neg_fseek_nolock_default_handler_fatal
 * area:  stdio_file
 * funcs: _fseek_nolock
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: default handler terminates
 * note:  Child process only; default handler terminates on msvcr80+ (exact status not asserted).
 *        Children now call tc_iph_default() first (class F: the mingw startup no-op handler otherwise hides the default handler).
 *        OBSERVED on ucrtbase (Win10 22H2 19045 reference integration full run triage-r1; and triage/crt-wp1-stdio-0 for gsf_neg_fseek_nolock_bad_args):
 *        _fseek_nolock does NOT validate origin (no handler call, returns 0), so even with the default handler the bad-origin child
 *        returns normally (77). Class A, deviates from the MS docs (Parameter validation): bad_origin asserts the child survives.
 *        The NULL stream is not validated either: it faults (not 0, not 77).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

int __cdecl _fseek_nolock(FILE *, long, int);
static FILE *volatile nullfp;
static volatile int vorg;

TC_CASE(null_stream_fatal)
{
    DWORD code = 0; CHECK(tc_spawn_self("nullseek", &code, 20000));
    CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(bad_origin_not_validated)
{
#if !defined(TC_UCRT)
    TC_SKIP("bad whence fatal only asserted for ucrtbase");
#else
    DWORD code = 0; CHECK(tc_spawn_self("badorigin", &code, 20000));
    CHECK_EQ_I(code, 77);   /* observed on Win10 22H2: origin 3 is accepted, the call returns, child exits 77 */
#endif
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();
        if (c[0] == 'n') { _fseek_nolock(nullfp, 0, SEEK_SET); }
        else { FILE *f = fopen("tc_gsf_neg_fseek_nolock_default_handler_fatal.tmp", "wb"); vorg = 3; if (f) _fseek_nolock(f, 0, vorg); }
        return 77;
    }
    TC_RUN(null_stream_fatal); TC_RUN(bad_origin_not_validated);
    remove("tc_gsf_neg_fseek_nolock_default_handler_fatal.tmp");
    return tc_end();
}
