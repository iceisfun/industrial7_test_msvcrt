/* TC
 * id:    loc_wcreate_locale_basic
 * area:  locale
 * funcs: _wcreate_locale _free_locale _isalpha_l
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _create_locale, _wcreate_locale
 * note:  Fix (win10 triage): added the missing _wcreate_locale prototype (implicit int truncated the returned pointer -> access violation).
 *        Invalid wide locale name returns NULL (documented). 0xE9 (e acute) is alpha in a 1252 locale.
 */
#include "tc.h"
#include <locale.h>
#include <ctype.h>
/* mingw headers do not declare _wcreate_locale; without a prototype the call returns int and the pointer is truncated (build defect) */
_locale_t __cdecl _wcreate_locale(int, const wchar_t *);
/* mingw's ctype.h defines these as inline macros; undefine so the DLL's exports are what is called */
#undef _isalpha_l
#undef _isupper_l
#undef _islower_l
#undef _isdigit_l
#undef _isxdigit_l
#undef _isspace_l
#undef _ispunct_l
#undef _isalnum_l
#undef _isprint_l
#undef _isgraph_l
#undef _iscntrl_l
#undef _tolower
#undef _toupper

static const wchar_t *volatile v_name = L"English_United States.1252";
static const wchar_t *volatile v_bad = L"zz_nowhere";

TC_CASE(wide_create_ok)
{
    _locale_t l = _wcreate_locale(LC_ALL, v_name);
    CHECK(l != NULL);
    if (l) _free_locale(l);
}
TC_CASE(classification_0xE9)
{
    _locale_t l = _wcreate_locale(LC_ALL, v_name);
    volatile int c = 0xE9, a = 'a', d = '5';
    if (!l) TC_SKIP("locale not available");
    CHECK(_isalpha_l(c, l) != 0);
    CHECK(_isalpha_l(a, l) != 0);
    CHECK(_isalpha_l(d, l) == 0);
    _free_locale(l);
}
TC_CASE(wide_invalid_null)
{
    _locale_t l = _wcreate_locale(LC_ALL, v_bad);
    CHECK(l == NULL);
    if (l) _free_locale(l);
}
int main(void) { tc_begin(); TC_RUN(wide_create_ok); TC_RUN(classification_0xE9); TC_RUN(wide_invalid_null); return tc_end(); }
