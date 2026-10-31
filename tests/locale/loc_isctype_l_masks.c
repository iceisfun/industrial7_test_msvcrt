/* TC
 * id:    loc_isctype_l_masks
 * area:  locale
 * funcs: _isctype _isctype_l
 * procs: _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _isctype, _isctype_l; ctype.h mask values
 * note:  _isctype returns nonzero when any bit of mask matches. Masks: _UPPER=1 _LOWER=2 _DIGIT=4 _SPACE=8 _PUNCT=0x10 _CONTROL=0x20 _BLANK=0x40 _HEX=0x80 _ALPHA=0x103.
 */
#include "tc.h"
#include <locale.h>
typedef _locale_t (__cdecl *tc_cl_t)(int, const char *);
typedef void (__cdecl *tc_fl_t)(_locale_t);
static tc_cl_t p_create_locale;
static tc_fl_t p_free_locale;
#include <ctype.h>
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

static _locale_t loc;
static const char *volatile v_c = "C";

TC_CASE(upper_mask)
{
    volatile int c = 'A';
    CHECK(_isctype_l(c, _UPPER, loc) != 0);
    CHECK(_isctype_l(c, _LOWER, loc) == 0);
    CHECK(_isctype_l(c, _ALPHA, loc) != 0);
    CHECK(_isctype_l(c, _HEX, loc) != 0);
    CHECK(_isctype_l(c, _DIGIT, loc) == 0);
}
TC_CASE(digit_hex_mask)
{
    volatile int c = '5';
    CHECK(_isctype_l(c, _DIGIT | _HEX, loc) != 0);
    CHECK(_isctype_l(c, _DIGIT, loc) != 0);
    CHECK(_isctype_l(c, _HEX, loc) != 0);
    CHECK(_isctype_l(c, _ALPHA, loc) == 0);
}
TC_CASE(space_mask)
{
    volatile int c = ' ', n = '\n';
    CHECK(_isctype_l(c, _SPACE, loc) != 0);
    CHECK(_isctype_l(n, _SPACE, loc) != 0);
    CHECK(_isctype_l(n, _CONTROL, loc) != 0);
    CHECK(_isctype_l(c, _CONTROL, loc) == 0);
}
TC_CASE(any_of_semantics_mixed_mask)
{
    volatile int c = 'A', z = 'z';
    CHECK(_isctype_l(c, _DIGIT | _UPPER, loc) != 0);
    CHECK(_isctype_l(c, _DIGIT | _SPACE | _PUNCT, loc) == 0);
    CHECK(_isctype_l(z, _UPPER | _LOWER, loc) != 0);
    CHECK(_isctype_l(z, _UPPER | _DIGIT, loc) == 0);
}
TC_CASE(isctype_global_agrees)
{
    int i, bad = -1; int masks[] = { _UPPER, _LOWER, _DIGIT, _SPACE, _PUNCT, _CONTROL, _HEX, _ALPHA };
    unsigned m;
    for (i = 0; i < 128 && bad < 0; i++)
        for (m = 0; m < sizeof masks / sizeof masks[0]; m++) { volatile int c = i;
            if ((_isctype(c, masks[m]) != 0) != (_isctype_l(c, masks[m], loc) != 0)) { bad = i; break; } }
    CHECK_EQ_I(bad, -1);
}
int main(void)
{
    tc_begin();
    p_create_locale = (tc_cl_t)tc_crt_proc("_create_locale"); p_free_locale = (tc_fl_t)tc_crt_proc("_free_locale");
    if (!p_create_locale || !p_free_locale) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    loc = p_create_locale(LC_ALL, v_c);
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create C locale\n"); return 1; }
    TC_RUN(upper_mask); TC_RUN(digit_hex_mask); TC_RUN(space_mask); TC_RUN(any_of_semantics_mixed_mask); TC_RUN(isctype_global_agrees);
    p_free_locale(loc);
    return tc_end();
}
