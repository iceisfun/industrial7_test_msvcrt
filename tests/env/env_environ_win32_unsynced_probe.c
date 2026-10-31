/* TC
 * id:    env_environ_win32_unsynced_probe
 * area:  env
 * funcs: getenv _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs getenv ("getenv operates only on the data structures accessible to the run-time library and not on the environment segment created for the process by the operating system")
 * note:  Probes: a variable set only through kernel32 SetEnvironmentVariable after startup is NOT seen by getenv, also
 *        not after an unrelated _putenv call. Variable is cleaned up at the end.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(win32_only_invisible)
{
    CHECK(SetEnvironmentVariableA("TC_UNSYNC_A", "w32only"));
    CHECK(getenv("TC_UNSYNC_A") == NULL);
}
TC_CASE(still_invisible_after_putenv)
{
    CHECK_EQ_I(_putenv("TC_UNSYNC_OTHER=x"), 0);
    CHECK(getenv("TC_UNSYNC_A") == NULL);
    CHECK_STR(getenv("TC_UNSYNC_OTHER"), "x");
    _putenv("TC_UNSYNC_OTHER=");
}
int main(void)
{
    tc_begin();
    TC_RUN(win32_only_invisible); TC_RUN(still_invisible_after_putenv);
    SetEnvironmentVariableA("TC_UNSYNC_A", NULL);
    return tc_end();
}
