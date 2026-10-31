/* TC
 * id:    sec_wcscat_s_basic
 * area:  secure_crt
 * funcs: wcscat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcat_s, wcscat_s
 */
#include "tc.h"
#include <wchar.h>

static volatile const wchar_t *v_s;

TC_CASE(append) { wchar_t d[16] = L"foo"; v_s = L"bar"; CHECK_EQ_I(wcscat_s(d, 16, (const wchar_t *)v_s), 0); CHECK_WSTR(d, L"foobar"); }
TC_CASE(append_empty) { wchar_t d[16] = L"foo"; v_s = L""; CHECK_EQ_I(wcscat_s(d, 16, (const wchar_t *)v_s), 0); CHECK_WSTR(d, L"foo"); }
TC_CASE(exact_fit) { wchar_t d[7] = L"foo"; v_s = L"bar"; CHECK_EQ_I(wcscat_s(d, 7, (const wchar_t *)v_s), 0); CHECK_WSTR(d, L"foobar"); }
TC_CASE(build_from_empty) { wchar_t d[8]; d[0] = 0; v_s = L"xyz"; CHECK_EQ_I(wcscat_s(d, 8, (const wchar_t *)v_s), 0); CHECK_WSTR(d, L"xyz"); }

int main(void) { tc_begin(); TC_RUN(append); TC_RUN(append_empty); TC_RUN(exact_fit); TC_RUN(build_from_empty); return tc_end(); }
