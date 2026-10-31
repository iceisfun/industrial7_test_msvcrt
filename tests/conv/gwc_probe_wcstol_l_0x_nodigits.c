/* TC
 * id:    gwc_probe_wcstol_l_0x_nodigits
 * area:  conv
 * funcs: _wcstol_l _wcstoul_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.4 (longest initial subsequence of expected form: "0")
 * note:  Probing "0x" with no hex digits: C says value 0 and endptr just after the '0'. Some old CRTs are believed to
 *        reset endptr to the start of the string when the prefix is consumed but no digit follows.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static long  S(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstol_l(vp, e, b, L); }
static unsigned long U(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoul_l(vp, e, b, L); }

TC_CASE(base16) { const wchar_t *s = L"0xg"; wchar_t *e; CHECK_EQ_I(S(s, &e, 16), 0); CHECK_EQ_P(e, s + 1); }
TC_CASE(base0) { const wchar_t *s = L"0xg"; wchar_t *e; CHECK_EQ_I(S(s, &e, 0), 0); CHECK_EQ_P(e, s + 1); }
TC_CASE(unsigned_base16) { const wchar_t *s = L"0x"; wchar_t *e; CHECK_EQ_U(U(s, &e, 16), 0); CHECK_EQ_P(e, s + 1); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(base16); TC_RUN(base0); TC_RUN(unsigned_base16);
    _free_locale(L);
    return tc_end();
}
