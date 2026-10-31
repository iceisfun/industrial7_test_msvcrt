/* TC
 * id:    cv_neg_ltow_ultow_radix_bounds
 * area:  conv
 * funcs: _ltow _ultow _i64tow _ui64tow
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa, _itow ("radix: base of value, must be in the range 2-36")
 * note:  Sign is emitted only for radix 10 (negative values with other radices are treated as unsigned), so negatives are checked in radix 10. Boundary-valid radices 2 and 36 (the largest/smallest that must NOT fail).
 */
#include "tc.h"
#include <stdlib.h>
#include <stdint.h>

TC_CASE(radix36)
{
    wchar_t w[80];
    _ultow(35, w, 36); CHECK_WSTR(w, L"z");
    _ltow(-35, w, 10); CHECK_WSTR(w, L"-35");
    _ultow(36, w, 36); CHECK_WSTR(w, L"10");
    _i64tow(-35, w, 10); CHECK_WSTR(w, L"-35");
    _ui64tow(35, w, 36); CHECK_WSTR(w, L"z");
}
TC_CASE(radix2)
{
    wchar_t w[80];
    _ultow(5, w, 2); CHECK_WSTR(w, L"101");
    _ltow(-5, w, 10); CHECK_WSTR(w, L"-5");
    _ui64tow(UINT64_MAX, w, 2);
    CHECK_EQ_U(wcslen(w), 64);
}

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(radix36); TC_RUN(radix2);
    return tc_end();
}
