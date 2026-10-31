/* TC
 * id:    pfl_probe_create_locale_overlong_name
 * area:  stdio_printf
 * kind:  positive
 * funcs: _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _create_locale (NULL for an invalid locale name)
 * note:  PROBE: a 399-character nonsense locale name is rejected with NULL (and does not crash). Not spelled out in the docs.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

TC_CASE(overlong_name_null)
{
    char n[400]; _locale_t l; int i;
    for (i = 0; i < 399; i++) n[i] = 'x';
    n[399] = 0;
    l = _create_locale(LC_ALL, n);
    CHECK(l == NULL); if (l) _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(overlong_name_null);
    return tc_end();
}
