/* TC
 * id:    gwc_probe_wcstoul_l_negative_overflow
 * area:  conv
 * funcs: _wcstoul_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.4 (value out of range of unsigned long -> ULONG_MAX, ERANGE)
 * note:  Probing "-4294967296" (magnitude exceeds ULONG_MAX): expected ULONG_MAX with ERANGE; whether the CRT
 *        reports overflow before applying the negation is the unknown.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static unsigned long U(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoul_l(vp, e, b, L); }

TC_CASE(neg_over) { wchar_t *e; errno = 0; CHECK_EQ_U(U(L"-4294967296", &e, 10), ULONG_MAX); CHECK_EQ_I(errno, ERANGE); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(neg_over);
    _free_locale(L);
    return tc_end();
}
