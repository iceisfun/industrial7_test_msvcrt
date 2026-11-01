/* TC
 * id:    env_neg_probe_putenv_embedded_equals_name_lookup
 * area:  env
 * kind:  negative
 * funcs: _putenv getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _putenv (name is the text before the first '=')
 * note:  _putenv("TC_EQ2=B=C") splits at the first "=": getenv("TC_EQ2") is "B=C". OBSERVED on Win10 22H2:
 *        GetEnvironmentVariableA("TC_EQ2=B") (a name containing "=") also succeeds and returns the value "C" (length 1): the OS
 *        lookup matches a name prefix followed by "=" in the stored "TC_EQ2=B=C" entry.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(splits_at_first_equals)
{
    CHECK_EQ_I(_putenv("TC_EQ2=B=C"), 0);
    CHECK_STR(getenv("TC_EQ2"), "B=C");
}
TC_CASE(os_matches_name_prefix)
{
    char buf[8]; DWORD n;
    n = GetEnvironmentVariableA("TC_EQ2=B", buf, sizeof buf);
    CHECK_EQ_U(n, 1);
    CHECK_STR(buf, "C");
    _putenv("TC_EQ2=");
}
int main(void)
{
    tc_begin();
    TC_RUN(splits_at_first_equals); TC_RUN(os_matches_name_prefix);
    return tc_end();
}
