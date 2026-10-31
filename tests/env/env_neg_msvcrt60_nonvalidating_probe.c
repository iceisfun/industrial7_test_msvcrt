/* TC
 * id:    env_neg_msvcrt60_nonvalidating_probe
 * area:  env
 * funcs: getenv _putenv
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _putenv (returns 0 on success, -1 on error)
 * note:  PROBE: legacy msvcrt.dll has no invalid-parameter handler. Probed: _putenv of a string without '=' does not crash
 *        and returns -1 (or 0), and does not create a variable; getenv("") returns NULL. No NULL arguments are passed.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(_putenv_no_equals_returns_minus1_or_0)
{
    int r = _putenv("TC_ENV_NOEQ_XYZ");
    CHECK(r == -1 || r == 0);
    CHECK(getenv("TC_ENV_NOEQ_XYZ") == NULL);
}
TC_CASE(getenv_empty_string_null)
{
    volatile const char *name = "";
    CHECK(getenv((const char *)name) == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(_putenv_no_equals_returns_minus1_or_0); TC_RUN(getenv_empty_string_null);
    return tc_end();
}
