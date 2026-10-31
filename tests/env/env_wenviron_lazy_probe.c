/* TC
 * id:    env_wenviron_lazy_probe
 * area:  env
 * funcs: __p__wenviron __p__environ _wgetenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _environ, _wenviron ("_wenviron is initially NULL in a main() program until a wide-character environment function is called")
 * note:  Probes: after _wgetenv the wide environment table exists and is NULL-terminated with at least one entry.
 *        The initial value (NULL or not) is deliberately not asserted.
 */
#include "tc.h"
#include <stdlib.h>

wchar_t ***__cdecl __p__wenviron(void);
char ***__cdecl __p__environ(void);

TC_CASE(pointer_valid)
{
    CHECK(__p__wenviron() != NULL);
    CHECK(__p__environ() != NULL);
    CHECK_EQ_P(__p__wenviron(), __p__wenviron());
}
TC_CASE(after_wgetenv)
{
    wchar_t **e; int n = 0;
    (void)_wgetenv(L"PATH");
    e = *__p__wenviron();
    CHECK(e != NULL);
    while (e && e[n]) n++;
    CHECK(n >= 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(pointer_valid); TC_RUN(after_wgetenv);
    return tc_end();
}
