/* TC
 * id:    pfi_neg_sprintf_default_handler_fatal
 * area:  stdio_printf
 * funcs: sprintf
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs Parameter validation: default handler terminates; ucrt uses fast-fail
 * note:  Child process; no handler installed. Exact status 0xC0000409 (fast-fail) is observed behaviour on Win10, not spelled out by docs;
 *        it would differ on Win7 + UCRT update (no fast-fail, 0xC0000417). Parent asserts 0xC0000409 and that the child never reached return 77.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(null_buffer_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullbuf", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char *volatile p = NULL; volatile int v = 1;
        sprintf(p, "%d", v);
        return 77; /* reached only if the CRT did not terminate */
    }
    TC_RUN(null_buffer_terminates);
    return tc_end();
}
