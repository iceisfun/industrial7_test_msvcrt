/* TC
 * id:    gwc_is_wctype_mask_consistency
 * area:  conv
 * funcs: is_wctype
 * procs: _iswalnum_l _create_locale _free_locale
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs iswctype (is_wctype is the old name), ctype.h mask constants _UPPER _LOWER _DIGIT _SPACE _PUNCT _CONTROL _HEX _ALPHA
 * note:  Expected classes are computed from the ASCII definitions of the C standard, not from other CRT functions. The
 *        _iswalnum_l relation case is skipped where the DLL does not export _iswalnum_l / _create_locale (msvcrt.dll, probably).
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

TC_DECL_PROC(int, _iswalnum_l, (unsigned int, void *));
TC_DECL_PROC(void *, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (void *));

static volatile wint_t vc;
static volatile unsigned short vm;
static int isw(wint_t c, unsigned short m) { vc = c; vm = m; return is_wctype(vc, vm) != 0; }

static int e_digit(int c) { return c >= '0' && c <= '9'; }
static int e_upper(int c) { return c >= 'A' && c <= 'Z'; }
static int e_lower(int c) { return c >= 'a' && c <= 'z'; }
static int e_space(int c) { return c == ' ' || (c >= 9 && c <= 13); }
static int e_cntrl(int c) { return c < 32 || c == 127; }
static int e_hex(int c)   { return e_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
static int e_punct(int c) { return c > 32 && c < 127 && !e_digit(c) && !e_upper(c) && !e_lower(c); }

TC_CASE(digit_upper_lower)
{
    int c, bad = 0;
    for (c = 0; c < 128; c++) {
        if (isw(c, _DIGIT) != e_digit(c)) bad++;
        if (isw(c, _UPPER) != e_upper(c)) bad++;
        if (isw(c, _LOWER) != e_lower(c)) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(space_punct_control_hex)
{
    int c, bad = 0;
    for (c = 0; c < 128; c++) {
        if (isw(c, _SPACE) != e_space(c)) bad++;
        if (isw(c, _PUNCT) != e_punct(c)) bad++;
        if (isw(c, _CONTROL) != e_cntrl(c)) bad++;
        if (isw(c, _HEX) != e_hex(c)) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(alpha_mask)
{
    int c, bad = 0;
    for (c = 0; c < 128; c++) {
        if (isw(c, _ALPHA) != (e_upper(c) || e_lower(c))) bad++;
        if (isw(c, _ALPHA | _DIGIT) != (e_upper(c) || e_lower(c) || e_digit(c))) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(alnum_l_matches_mask)
{
    int c, bad = 0; void *loc;
    if (!tc_p__iswalnum_l || !tc_p__create_locale) TC_SKIP("_iswalnum_l/_create_locale not exported");
    loc = tc_p__create_locale(0, "C");
    CHECK(loc != NULL);
    for (c = 0; c < 128; c++)
        if ((tc_p__iswalnum_l((unsigned)c, loc) != 0) != isw(c, _ALPHA | _DIGIT)) bad++;
    CHECK_EQ_I(bad, 0);
    if (loc && tc_p__free_locale) tc_p__free_locale(loc);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_iswalnum_l); TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale);
    TC_RUN(digit_upper_lower); TC_RUN(space_punct_control_hex); TC_RUN(alpha_mask);
    TC_RUN(alnum_l_matches_mask);
    return tc_end();
}
