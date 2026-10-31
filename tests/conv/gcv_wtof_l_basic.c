/* TC
 * id:    gcv_wtof_l_basic
 * area:  conv
 * funcs: _wtof_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs atof, _atof_l, _wtof, _wtof_l
 * note:  
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t L;
static double F(const wchar_t *volatile s) { return _wtof_l(s, L); }

TC_CASE(simple) { CHECK_BITS_D(F(L"1.5"), 1.5); CHECK_BITS_D(F(L"-2.25"), -2.25); CHECK_BITS_D(F(L"1e3"), 1000.0); CHECK_BITS_D(F(L".5"), 0.5); CHECK_BITS_D(F(L"5."), 5.0); }
TC_CASE(ws_junk) { CHECK_BITS_D(F(L" \t\n 3.5abc"), 3.5); CHECK_BITS_D(F(L"+0.125xyz"), 0.125); }
TC_CASE(comma) { CHECK_BITS_D(F(L"1,5"), 1.0); }
TC_CASE(zeros) { CHECK_BITS_D(F(L"0"), 0.0); CHECK_BITS_D(F(L"0.0e5"), 0.0); }

int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(simple); TC_RUN(ws_junk); TC_RUN(comma); TC_RUN(zeros);
    _free_locale(L);
    return tc_end();
}
