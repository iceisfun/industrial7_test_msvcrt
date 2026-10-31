/* TC
 * id:    gst_wcstof_neg_range_null
 * area:  conv
 * funcs: wcstof _wcstof_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * note:  NULL-nptr cases run on ucrtbase only; msvcr120 is probed in gst_strtof_family_neg_null_probe.
 * ref:   C11 7.29.4.1.1; MS docs wcstof (HUGE_VAL/0 with ERANGE), Parameter validation (NULL: EINVAL + handler; default handler fatal)
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

static _locale_t loc;
static const wchar_t *volatile v;

TC_CASE(overflow)
{
    errno = 0; CHECK_EQ_U(tc_fbits(wcstof(v = L"1e39", NULL)), 0x7f800000u); CHECK_EQ_I(errno, ERANGE);
    errno = 0; CHECK_EQ_U(tc_fbits(wcstof(v = L"-1e39", NULL)), 0xff800000u); CHECK_EQ_I(errno, ERANGE);
    errno = 0; CHECK_EQ_U(tc_fbits(_wcstof_l(v = L"1e39", NULL, loc)), 0x7f800000u); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(underflow) { errno = 0; CHECK_EQ_U(tc_fbits(wcstof(v = L"1e-60", NULL)), 0u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(no_conversion)
{
    wchar_t *e;
    v = L"abc"; e = (wchar_t *)1; CHECK_EQ_U(tc_fbits(wcstof(v, &e)), 0u); CHECK_EQ_P(e, v);
    v = L"";    e = (wchar_t *)1; CHECK_EQ_U(tc_fbits(wcstof(v, &e)), 0u); CHECK_EQ_P(e, v);
}
TC_CASE(null_nptr)
{
    float f; wchar_t *e = (wchar_t *)1;
    if (!TC_AT_LEAST(140)) TC_SKIP("msvcr120 NULL handling is in gst_strtof_family_neg_null_probe");
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler");
    tc_iph_count = 0; errno = 0; v = NULL; f = wcstof(v, &e);
    CHECK_EQ_U(tc_fbits(f), 0u); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; f = _wcstof_l(v, NULL, loc);
    CHECK_EQ_U(tc_fbits(f), 0u); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(sanity) { DWORD code = 0; CHECK(tc_spawn_self("sanity", &code, 20000)); CHECK_EQ_U(code, 77); }
TC_CASE(fatal_default) { DWORD code = 0; int ran; if (!TC_AT_LEAST(140)) TC_SKIP("ucrtbase only");
    ran = tc_spawn_self("wcstof_null", &code, 20000); CHECK(ran); CHECK(code != 0); CHECK(code != 77); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 's') return 77;
        (void)wcstof(NULL, NULL);
        return 77;
    }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(overflow); TC_RUN(underflow); TC_RUN(no_conversion); TC_RUN(null_nptr); TC_RUN(sanity); TC_RUN(fatal_default);
    _free_locale(loc);
    return tc_end();
}
