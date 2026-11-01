/* TC
 * id:    env_probe_putenv_exported
 * area:  env
 * funcs:
 * procs: putenv
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _putenv ("putenv" is a deprecated POSIX name)
 * note:  Whether the DLL itself exports the POSIX name "putenv". OBSERVED on Win10 22H2: ucrtbase.dll does NOT export "putenv"
 *        (only _putenv; putenv lives in the static oldnames library). Expected exported on msvcrt.dll and the older
 *        redistributables, so the assertion is restricted to ucrtbase.
 */
#include "tc.h"

TC_CASE(putenv_export_absent)
{
    CHECK(tc_crt_proc("putenv") == NULL);   /* observed: ucrtbase.dll does not export the POSIX name */
}

int main(void)
{
    tc_begin();
    TC_RUN(putenv_export_absent);
    return tc_end();
}
