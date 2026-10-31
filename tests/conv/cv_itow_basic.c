/* TC
 * id:    cv_itow_basic
 * area:  conv
 * funcs: _itow _ltow _ultow _i64tow _ui64tow
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _itoa, _itow
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile int v_i; static volatile long v_l; static volatile unsigned long v_ul; static volatile __int64 v_64; static volatile unsigned __int64 v_u64;

TC_CASE(itow_dec) { wchar_t b[40]; v_i = -123; _itow(v_i, b, 10); CHECK_WSTR(b, L"-123"); }
TC_CASE(itow_hex) { wchar_t b[40]; v_i = 255; _itow(v_i, b, 16); CHECK_WSTR(b, L"ff"); }
TC_CASE(ltow) { wchar_t b[40]; v_l = -123456L; _ltow(v_l, b, 10); CHECK_WSTR(b, L"-123456"); }
TC_CASE(ultow_max) { wchar_t b[40]; v_ul = ULONG_MAX; _ultow(v_ul, b, 10); CHECK_WSTR(b, L"4294967295"); }
TC_CASE(i64tow_min) { wchar_t b[40]; v_64 = -9223372036854775807LL - 1; _i64tow(v_64, b, 10); CHECK_WSTR(b, L"-9223372036854775808"); }
TC_CASE(ui64tow_base2) { wchar_t b[70]; v_u64 = 5; _ui64tow(v_u64, b, 2); CHECK_WSTR(b, L"101"); }
TC_CASE(returns_buffer) { wchar_t b[40]; v_i = 7; CHECK(_itow(v_i, b, 10) == b); CHECK(_ltow(7L, b, 10) == b); CHECK(_ultow(7UL, b, 10) == b); CHECK(_i64tow(7, b, 10) == b); CHECK(_ui64tow(7, b, 10) == b); }

int main(void)
{
    tc_begin();
    TC_RUN(itow_dec); TC_RUN(itow_hex); TC_RUN(ltow); TC_RUN(ultow_max); TC_RUN(i64tow_min); TC_RUN(ui64tow_base2); TC_RUN(returns_buffer);
    return tc_end();
}
