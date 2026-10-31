/* TC
 * id:    pfl_probe_sprintf_l_dbcs_width_bytes
 * area:  stdio_printf
 * kind:  positive
 * funcs: _snprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf width specification
 * note:  PROBE: in a DBCS locale (Japanese_Japan.932) the width of a narrow %s is measured in bytes, not characters:
 *        "[%6s]" with one double-byte character (2 bytes) gives 4 pad bytes. Skips if the locale is unavailable.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

TC_CASE(cp932_width_counts_bytes)
{
    _locale_t l = _create_locale(LC_ALL, "Japanese_Japan.932"); char b[32]; int r;
    volatile const char *s = "\x93\xFA";
    if (!l) TC_SKIP("Japanese locale not installed");
    r = _snprintf_l(b, sizeof b, "[%6s]", l, (const char *)s);
    CHECK_EQ_I(r, 8);
    CHECK_MEM(b, "[    \x93\xFA]", 8);
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(cp932_width_counts_bytes);
    return tc_end();
}
