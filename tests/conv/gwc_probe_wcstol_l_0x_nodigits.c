/* TC
 * id:    gwc_probe_wcstol_l_0x_nodigits
 * area:  conv
 * funcs: _wcstol_l _wcstoul_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.22.1.4 (longest initial subsequence of expected form: "0")
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): _wcstol_l/_wcstoul_l of L"0xg" / L"0x" (base 16, base 0) return 0 with endptr == nptr.
 *        deviates from C11 7.29.4.1.2p4 (the "0" before a prefix-only "0x" is not converted; endptr == nptr).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

#if TC_AT_LEAST(140)
#define EXP_END(p) (p)       /* observed Win10 22H2 ucrtbase: nothing consumed */
#else
#define EXP_END(p) ((p) + 1) /* ISO C; unverified on pre-UCRT CRTs */
#endif
static _locale_t L;
static const wchar_t *volatile vp;
static long  S(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstol_l(vp, e, b, L); }
static unsigned long U(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoul_l(vp, e, b, L); }

TC_CASE(base16) { const wchar_t *s = L"0xg"; wchar_t *e; CHECK_EQ_I(S(s, &e, 16), 0); CHECK_EQ_P(e, EXP_END(s)); }
TC_CASE(base0) { const wchar_t *s = L"0xg"; wchar_t *e; CHECK_EQ_I(S(s, &e, 0), 0); CHECK_EQ_P(e, EXP_END(s)); }
TC_CASE(unsigned_base16) { const wchar_t *s = L"0x"; wchar_t *e; CHECK_EQ_U(U(s, &e, 16), 0); CHECK_EQ_P(e, EXP_END(s)); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(base16); TC_RUN(base0); TC_RUN(unsigned_base16);
    _free_locale(L);
    return tc_end();
}
