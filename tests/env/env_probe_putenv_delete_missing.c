/* TC
 * id:    env_probe_putenv_delete_missing
 * area:  env
 * funcs: _putenv getenv
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putenv
 * note:  PROBE: _putenv("NAME=") for a variable that does not exist returns 0 (deleting nothing is not an error) and getenv stays NULL.
 *        Split from env_putenv_delete: the docs do not say whether the failing SetEnvironmentVariable(ERROR_ENVVAR_NOT_FOUND)
 *        of the underlying delete is reported as -1 by any CRT generation.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(delete_missing_returns_0)
{
    CHECK(getenv("TC_PD_NOSUCH") == NULL);
    CHECK_EQ_I(_putenv("TC_PD_NOSUCH="), 0);
    CHECK(getenv("TC_PD_NOSUCH") == NULL);
}
int main(void) { tc_begin(); TC_RUN(delete_missing_returns_0); return tc_end(); }
