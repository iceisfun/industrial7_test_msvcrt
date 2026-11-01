/* TC
 * id:    dat_neg_get_tzname_fatal
 * area:  data_exports
 * kind:  negative
 * funcs: _get_tzname
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs "Parameter validation": default invalid parameter handler terminates the process
 * note:  Probes the exact termination status (expected fast-fail 0xC0000409) when _get_tzname gets index 5 with no handler installed.
 * note:  Class F, Win10 22H2 19045; raw "code == TC_STATUS_FASTFAIL got=0x4d"): the MinGW startup handler returns, so the child ran to its own exit 77.
 *        The child now restores the CRT default handler (tc_iph_default); the assertion (fast-fail 0xC0000409) is unchanged.
 */
#include "tc.h"
#include <time.h>

TC_CASE(child_terminates_fastfail)
{
    DWORD code = 0; int ran = tc_spawn_self("badidx", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { size_t r; char b[8]; tc_iph_default(); volatile int idx = 5; _get_tzname(&r, b, sizeof b, idx); return 77; }
    TC_RUN(child_terminates_fastfail);
    return tc_end();
}
