/* TC
 * id:    ucr_neg_abort_default_exitcode_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: abort
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs abort (exit code 3); observed UCRT behaviour of fast-failing from abort
 * note:  Probes the exit code of abort() with default _set_abort_behavior flags. The assertion is that it is NOT 77 (child
 *        returned) and is either 3 or 0xC0000409 (STATUS_STACK_BUFFER_OVERRUN via __fastfail, the guess for retail ucrtbase).
 *        Child disables the WER dialog with SetErrorMode.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(default_abort)
{
    DWORD c = 0;
    CHECK(tc_spawn_self("abort", &c, 30000));
    CHECK(c == 3 || c == 0xC0000409u);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        abort();
        return 77;
    }
    TC_RUN(default_abort);
    return tc_end();
}
