/* TC
 * id:    prc_neg_system_comspec_nonexistent_win7_probe
 * area:  process
 * funcs: system _wsystem _putenv
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs system (uses COMSPEC, else cmd.exe searched on PATH)
 * note:  PROBE: with COMSPEC naming a missing file the CRT falls back to searching cmd.exe on PATH, so "exit 6" still returns 6.
 *        Done in child processes (environment change) with PATH untouched; wide and narrow variants are separate cases.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(wsystem_fallback) { DWORD c = 0; CHECK(tc_spawn_self("sysw", &c, 20000)); CHECK_EQ_U(c, 6); }
TC_CASE(system_fallback)  { DWORD c = 0; CHECK(tc_spawn_self("sysa", &c, 20000)); CHECK_EQ_U(c, 6); }
int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        _putenv("COMSPEC=C:\\tc_no_such_dir\\missing_cmd.exe");
        if (n[3] == 'w') return _wsystem(L"exit 6");
        return system("exit 6");
    }
    TC_RUN(wsystem_fallback); TC_RUN(system_fallback);
    return tc_end();
}
