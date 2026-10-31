/* TC
 * id:    ct_isw_l_variants
 * area:  ctype
 * funcs: 
 * procs: _iswalpha_l _iswdigit_l _iswspace_l _iswupper_l _iswlower_l _iswctype_l _towupper_l _towlower_l _create_locale _free_locale
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs isalpha, isalpha_l, iswalpha, _towupper_l, _iswctype_l
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

typedef int (__cdecl *iw_t)(unsigned short, void *);
typedef int (__cdecl *iwc_t)(unsigned short, unsigned short, void *);
typedef unsigned short (__cdecl *tw_t)(unsigned short, void *);
static iw_t p_alpha, p_digit, p_space, p_upper, p_lower;
static iwc_t p_ctype;
static tw_t p_up, p_low;
static void *(__cdecl *p_create)(int, const char *);
static void (__cdecl *p_free)(void *);
static void *loc;

TC_CASE(alpha_digit_space_ascii)
{
    loc = p_create ? p_create(LC_ALL, "C") : NULL;
    CHECK(loc != NULL); if (!loc || !p_alpha || !p_digit || !p_space) return;
    CHECK(p_alpha(L'a', loc)); CHECK(p_alpha(L'Z', loc)); CHECK(!p_alpha(L'1', loc)); CHECK(!p_alpha(L' ', loc));
    CHECK(p_digit(L'0', loc)); CHECK(p_digit(L'9', loc)); CHECK(!p_digit(L'a', loc));
    CHECK(p_space(L' ', loc)); CHECK(p_space(L'\n', loc)); CHECK(!p_space(L'x', loc));
}
TC_CASE(upper_lower_ascii)
{
    if (!loc || !p_upper || !p_lower) TC_SKIP("missing");
    CHECK(p_upper(L'Q', loc)); CHECK(!p_upper(L'q', loc)); CHECK(!p_upper(L'7', loc));
    CHECK(p_lower(L'q', loc)); CHECK(!p_lower(L'Q', loc)); CHECK(!p_lower(L'7', loc));
}
TC_CASE(towupper_towlower_ascii)
{
    if (!loc || !p_up || !p_low) TC_SKIP("missing");
    CHECK_EQ_I(p_up(L'a', loc), L'A'); CHECK_EQ_I(p_up(L'A', loc), L'A'); CHECK_EQ_I(p_up(L'1', loc), L'1');
    CHECK_EQ_I(p_low(L'Z', loc), L'z'); CHECK_EQ_I(p_low(L'z', loc), L'z'); CHECK_EQ_I(p_low(L'!', loc), L'!');
}
TC_CASE(iswctype_l_masks)
{
    if (!loc || !p_ctype) TC_SKIP("missing");
    CHECK(p_ctype(L'A', _ALPHA, loc)); CHECK(p_ctype(L'A', _UPPER, loc)); CHECK(!p_ctype(L'A', _LOWER, loc));
    CHECK(p_ctype(L'5', _DIGIT, loc)); CHECK(!p_ctype(L'5', _ALPHA, loc));
    CHECK(p_ctype(L' ', _SPACE, loc)); CHECK(p_ctype(L'!', _PUNCT, loc)); CHECK(p_ctype(L'f', _HEX, loc));
}
TC_CASE(free_locale) { if (loc && p_free) p_free(loc); loc = NULL; CHECK(1); }
int main(void)
{
    tc_begin();
    p_create = (void *(__cdecl *)(int, const char *))tc_crt_proc("_create_locale");
    p_free = (void (__cdecl *)(void *))tc_crt_proc("_free_locale");
    p_alpha = (iw_t)tc_crt_proc("_iswalpha_l"); p_digit = (iw_t)tc_crt_proc("_iswdigit_l");
    p_space = (iw_t)tc_crt_proc("_iswspace_l"); p_upper = (iw_t)tc_crt_proc("_iswupper_l");
    p_lower = (iw_t)tc_crt_proc("_iswlower_l"); p_ctype = (iwc_t)tc_crt_proc("_iswctype_l");
    p_up = (tw_t)tc_crt_proc("_towupper_l"); p_low = (tw_t)tc_crt_proc("_towlower_l");
    TC_RUN(alpha_digit_space_ascii); TC_RUN(upper_lower_ascii); TC_RUN(towupper_towlower_ascii); TC_RUN(iswctype_l_masks); TC_RUN(free_locale);
    return tc_end();
}
