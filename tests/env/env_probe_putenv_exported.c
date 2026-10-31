/* TC
 * id:    env_probe_putenv_exported
 * area:  env
 * funcs:
 * procs: putenv
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putenv ("putenv" is a deprecated POSIX name)
 * note:  PROBE: whether the DLL itself exports the POSIX-name "putenv" (vs. it living only in the static oldnames library).
 *        Expected exported on msvcrt.dll and the older redistributables; unknown for ucrtbase. Recorded only: the single assertion simply fails where putenv is not exported (that is the finding).
 */
#include "tc.h"

TC_CASE(putenv_export_present)
{
    CHECK(tc_crt_proc("putenv") != NULL);
}

int main(void)
{
    tc_begin();
    TC_RUN(putenv_export_present);
    return tc_end();
}
