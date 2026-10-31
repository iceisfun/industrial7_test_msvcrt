/* TC
 * id:    ct_pctype_func_tracks_locale
 * area:  ctype
 * funcs: __pctype_func setlocale isalpha
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs __pctype_func, setlocale, isalpha
 * note:  Re-fetch __pctype_func() after each setlocale. Skipped if cp1252 locale unavailable.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>
#undef __pctype_func
#undef isalpha

#define T_ALPHA 0x100
#define T_LOWER 0x02

static const unsigned short *tbl(void) { return (const unsigned short *)__pctype_func(); }

TC_CASE(c_locale_0xE9_no_alpha_bit)
{
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(tbl()[0xE9] & (T_ALPHA | T_LOWER), 0);
    CHECK(tbl()['a'] & T_LOWER);
}
TC_CASE(cp1252_0xE9_has_lower_and_alpha)
{
    if (!setlocale(LC_ALL, "English_United States.1252")) TC_SKIP("cp1252 locale unavailable");
    CHECK_EQ_U(tbl()[0xE9] & (T_ALPHA | T_LOWER), T_ALPHA | T_LOWER);
    CHECK(isalpha(0xE9) != 0);
    setlocale(LC_ALL, "C");
}
TC_CASE(pointer_refetched_after_setlocale)
{
    const unsigned short *a, *b;
    setlocale(LC_ALL, "C");
    a = tbl();
    if (!setlocale(LC_ALL, "English_United States.1252")) TC_SKIP("cp1252 locale unavailable");
    b = tbl();
    CHECK(b != NULL);
    CHECK(a != NULL);
    CHECK(b[0xC9] & 1); /* _UPPER */
    setlocale(LC_ALL, "C");
}
TC_CASE(back_to_c_clears_bits)
{
    if (!setlocale(LC_ALL, "English_United States.1252")) TC_SKIP("cp1252 locale unavailable");
    CHECK(tbl()[0xE9] & T_ALPHA);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(tbl()[0xE9] & (T_ALPHA | T_LOWER), 0);
    CHECK_EQ_I(isalpha(0xE9), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(c_locale_0xE9_no_alpha_bit); TC_RUN(cp1252_0xE9_has_lower_and_alpha);
    TC_RUN(pointer_refetched_after_setlocale); TC_RUN(back_to_c_clears_bits);
    return tc_end();
}
