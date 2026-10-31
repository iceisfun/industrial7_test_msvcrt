/* TC
 * id:    ext_neg_signal_invalid_default_iph_fatal
 * area:  exit_signal
 * kind:  negative
 * funcs: signal
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs signal / Parameter validation: default handler terminates the process
 * note:  Asserts only that the child terminates abnormally (not exit 0, not the 77 survival marker); exact status not asserted.
 */
#include "tc.h"
#include <signal.h>
static void __cdecl h(int s) { (void)s; }
TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("badsig", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { volatile int sn = 0; signal(sn, h); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
