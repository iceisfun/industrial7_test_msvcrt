/* TC
 * id:    env_neg_probe_putenv_embedded_equals_name_lookup
 * area:  env
 * kind:  negative
 * funcs: _putenv getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putenv (name is the text before the first '=')
 * note:  PROBE: _putenv("TC_EQ2=B=C") splits at the first '=': getenv("TC_EQ2") is "B=C" and the OS has no variable named "TC_EQ2=B".
 *        getenv("TC_EQ2=B") itself is NOT asserted: the CRT compares a name prefix then looks for '=', so it may match the entry.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(splits_at_first_equals)
{
    CHECK_EQ_I(_putenv("TC_EQ2=B=C"), 0);
    CHECK_STR(getenv("TC_EQ2"), "B=C");
}
TC_CASE(no_name_with_equals)
{
    char buf[8]; DWORD n;
    n = GetEnvironmentVariableA("TC_EQ2=B", buf, sizeof buf);
    CHECK_EQ_U(n, 0);
    _putenv("TC_EQ2=");
}
int main(void)
{
    tc_begin();
    TC_RUN(splits_at_first_equals); TC_RUN(no_name_with_equals);
    return tc_end();
}
