/* TC
 * id:    gst_wcstoimax_l_neg_range_base_null
 * area:  conv
 * funcs: _wcstoimax_l _wcstoumax_l wcstoumax _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstoimax/wcstoumax (ERANGE clamp, invalid base / NULL: invalid parameter handler, EINVAL); Parameter validation
 * note:  The fatal default-handler path for wcstoumax(NULL) runs in a child process.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
extern intmax_t __cdecl _wcstoimax_l(const wchar_t *, wchar_t **, int, _locale_t);
extern uintmax_t __cdecl _wcstoumax_l(const wchar_t *, wchar_t **, int, _locale_t);

static const wchar_t *volatile w;
static _locale_t loc;

TC_CASE(overflow)
{
    wchar_t *e;
    w = L"9223372036854775808"; errno = 0; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), INTMAX_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(e - w, 19);
    w = L"-9223372036854775809"; errno = 0; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), INTMAX_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(e - w, 20);
    w = L"18446744073709551616"; errno = 0; CHECK_EQ_U(_wcstoumax_l(w, &e, 10, loc), UINTMAX_MAX); CHECK_EQ_I(errno, ERANGE);
    errno = 0; CHECK_EQ_U(wcstoumax(w, &e, 10), UINTMAX_MAX); CHECK_EQ_I(errno, ERANGE);
    errno = 0;
}
TC_CASE(bad_base)
{
    wchar_t *e; static const int bases[] = { 1, 37 }; int i;
    w = L"123";
    for (i = 0; i < 2; i++) {
        tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wcstoimax_l(w, &e, bases[i], loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
        tc_iph_count = 0; errno = 0; CHECK_EQ_U(_wcstoumax_l(w, &e, bases[i], loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
        tc_iph_count = 0; errno = 0; CHECK_EQ_U(wcstoumax(w, &e, bases[i]), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    }
}
TC_CASE(null_nptr)
{
    wchar_t *e = (wchar_t *)1; const wchar_t *volatile n = NULL;
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wcstoimax_l(n, &e, 10, loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_U(_wcstoumax_l(n, &e, 10, loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_U(wcstoumax(n, &e, 10), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(no_digits)
{
    wchar_t *e;
    w = L"zzz"; e = (wchar_t *)1; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), 0); CHECK_EQ_P(e, w);
    e = (wchar_t *)1; CHECK_EQ_U(wcstoumax(w, &e, 10), 0); CHECK_EQ_P(e, w);
    w = L""; e = (wchar_t *)1; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), 0); CHECK_EQ_P(e, w);
    e = (wchar_t *)1; CHECK_EQ_U(_wcstoumax_l(w, &e, 10, loc), 0); CHECK_EQ_P(e, w);
}
TC_CASE(fatal_default_child)
{
    DWORD code = 0; int ran = tc_spawn_self("sanity", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
    ran = tc_spawn_self("wcstoumax_null", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'w') { const wchar_t *volatile n = NULL; (void)wcstoumax(n, NULL, 10); }
        return 77;
    }
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no_iph\n"); return 1; }
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(overflow); TC_RUN(bad_base); TC_RUN(null_nptr); TC_RUN(no_digits); TC_RUN(fatal_default_child);
    _free_locale(loc);
    return tc_end();
}
