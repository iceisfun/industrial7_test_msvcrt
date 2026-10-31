/* TC
 * id:    pfl_mbcs_percent_c_s_locale
 * area:  stdio_printf
 * funcs: _sprintf_l _snprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field characters (%c, %s: single-byte / multibyte); _create_locale; code page 1252 / 932
 * note:  Locales that are not installed make the case skip. No assertion is made about precision cutting a DBCS pair.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

TC_CASE(cp1252_high_byte_c)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); char b[16]; int r; volatile int ch = 0xE4;
    if (!l) TC_SKIP("German locale not installed");
    r = _sprintf_l(b, "[%c]", l, ch);
    CHECK_EQ_I(r, 3); CHECK_EQ_I((unsigned char)b[1], 0xE4); CHECK(b[0] == '[' && b[2] == ']');
    _free_locale(l);
}
TC_CASE(cp1252_high_byte_s)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); char b[16]; int r;
    volatile const char *s = "\xE4\xF6\xFC\xDF";
    if (!l) TC_SKIP("German locale not installed");
    r = _snprintf_l(b, sizeof b, "%s", l, (const char *)s);
    CHECK_EQ_I(r, 4); CHECK_MEM(b, "\xE4\xF6\xFC\xDF", 4); CHECK_EQ_I(b[4], 0);
    _free_locale(l);
}
TC_CASE(cp932_dbcs_s_preserved_skip_if_absent)
{
    _locale_t l = _create_locale(LC_ALL, "Japanese_Japan.932"); char b[16]; int r;
    volatile const char *s = "\x93\xFA\x96\x7B";  /* two double-byte characters */
    if (!l) TC_SKIP("Japanese locale not installed");
    r = _snprintf_l(b, sizeof b, "<%s>", l, (const char *)s);
    CHECK_EQ_I(r, 6); CHECK_MEM(b, "<\x93\xFA\x96\x7B>", 6); CHECK_EQ_I(b[6], 0);
    _free_locale(l);
}
int main(void)
{
    tc_begin();
    TC_RUN(cp1252_high_byte_c); TC_RUN(cp1252_high_byte_s); TC_RUN(cp932_dbcs_s_preserved_skip_if_absent);
    return tc_end();
}
