/* TC
 * id:    gst_wcstof_basic
 * area:  conv
 * funcs: wcstof _wcstof_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.29.4.1.1; MS docs strtof, wcstof, _wcstof_l
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <float.h>

static _locale_t loc;
static const wchar_t *volatile v;

TC_CASE(simple)
{
    v = L"1.5";  CHECK_EQ_U(tc_fbits(wcstof(v, NULL)), 0x3fc00000u);
    v = L"-0.0"; CHECK_EQ_U(tc_fbits(wcstof(v, NULL)), 0x80000000u);
    v = L"0.125"; CHECK_BITS_F(wcstof(v, NULL), 0.125f);
}
TC_CASE(endptr_units)
{
    wchar_t *e = NULL;
    v = L"  2.5e1abc"; CHECK_BITS_F(wcstof(v, &e), 25.0f); CHECK_EQ_P(e, v + 7); CHECK(*e == L'a');
}
TC_CASE(limits)
{
    v = L"3.4028234663852886e38"; CHECK_BITS_F(wcstof(v, NULL), FLT_MAX);
    v = L"1.17549435e-38";       CHECK_BITS_F(wcstof(v, NULL), FLT_MIN);
    v = L"3.4028234663852886e38"; CHECK_BITS_F(_wcstof_l(v, NULL, loc), FLT_MAX);
}
TC_CASE(matches_l_variant)
{
    static const wchar_t *const t[] = { L"0", L"-1.75", L"123.456", L"1e10", L"  8", L"+0.5x", L"1e-5", L".5", L"7." };
    size_t i;
    for (i = 0; i < sizeof t / sizeof t[0]; i++) {
        wchar_t *e1 = NULL, *e2 = NULL; float a, b; v = t[i];
        a = wcstof(v, &e1); b = _wcstof_l(v, &e2, loc);
        CHECK_EQ_U(tc_fbits(a), tc_fbits(b)); CHECK_EQ_P(e1, e2);
    }
}
TC_CASE(special_ucrt)
{
    wchar_t *e = NULL;
#ifdef TC_UCRT
    v = L"inf";     CHECK_EQ_U(tc_fbits(wcstof(v, &e)), 0x7f800000u); CHECK_EQ_P(e, v + 3);
    v = L"-infinity"; CHECK_EQ_U(tc_fbits(wcstof(v, &e)), 0xff800000u); CHECK_EQ_P(e, v + 9);
    v = L"nan";     { float f = wcstof(v, &e); CHECK(f != f); CHECK_EQ_P(e, v + 3); }
    v = L"0x1.8p1"; CHECK_BITS_F(wcstof(v, &e), 3.0f); CHECK_EQ_P(e, v + 7);
#else
    TC_SKIP("ucrtbase only");
#endif
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(simple); TC_RUN(endptr_units); TC_RUN(limits); TC_RUN(matches_l_variant); TC_RUN(special_ucrt);
    _free_locale(loc);
    return tc_end();
}
