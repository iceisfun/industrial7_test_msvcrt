/* TC
 * id:    sec_wcsncpy_s_basic
 * area:  secure_crt
 * funcs: wcsncpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strncpy_s, wcsncpy_s
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

static volatile const wchar_t *v_s;

TC_CASE(count_less) { wchar_t d[16]; v_s = L"abcdef"; CHECK_EQ_I(wcsncpy_s(d, 16, (const wchar_t *)v_s, 3), 0); CHECK_WSTR(d, L"abc"); }
TC_CASE(count_greater) { wchar_t d[16]; v_s = L"abcdef"; CHECK_EQ_I(wcsncpy_s(d, 16, (const wchar_t *)v_s, 100), 0); CHECK_WSTR(d, L"abcdef"); }
TC_CASE(truncate_fits) { wchar_t d[16]; v_s = L"abcdef"; CHECK_EQ_I(wcsncpy_s(d, 16, (const wchar_t *)v_s, (size_t)-1), 0); CHECK_WSTR(d, L"abcdef"); }
TC_CASE(truncate_exceeds_returns_80) { wchar_t d[4]; v_s = L"abcdef"; CHECK_EQ_I(wcsncpy_s(d, 4, (const wchar_t *)v_s, (size_t)-1), 80); CHECK_WSTR(d, L"abc"); }
TC_CASE(terminated) { wchar_t d[8]; int i; for (i = 0; i < 8; i++) d[i] = 0x7777; v_s = L"abcdef"; CHECK_EQ_I(wcsncpy_s(d, 8, (const wchar_t *)v_s, 2), 0); CHECK(d[2] == 0); }

int main(void) { tc_begin(); TC_RUN(count_less); TC_RUN(count_greater); TC_RUN(truncate_fits); TC_RUN(truncate_exceeds_returns_80); TC_RUN(terminated); return tc_end(); }
