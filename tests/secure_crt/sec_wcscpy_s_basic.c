/* TC
 * id:    sec_wcscpy_s_basic
 * area:  secure_crt
 * funcs: wcscpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcpy_s, wcscpy_s
 */
#include "tc.h"
#include <wchar.h>

static volatile const wchar_t *v_s;

TC_CASE(copy_simple) { wchar_t d[16]; v_s = L"hello"; CHECK_EQ_I(wcscpy_s(d, 16, (const wchar_t *)v_s), 0); CHECK_WSTR(d, L"hello"); }
TC_CASE(exact_fit) { wchar_t d[6]; v_s = L"hello"; CHECK_EQ_I(wcscpy_s(d, 6, (const wchar_t *)v_s), 0); CHECK_WSTR(d, L"hello"); }
TC_CASE(empty_src) { wchar_t d[4] = {1, 1, 1, 1}; v_s = L""; CHECK_EQ_I(wcscpy_s(d, 4, (const wchar_t *)v_s), 0); CHECK(d[0] == 0); }
TC_CASE(non_ascii_wchar_preserved) { wchar_t d[4]; static const wchar_t s[] = {0x4e2d, 0xfffd, 0x00e9, 0}; v_s = s;
    CHECK_EQ_I(wcscpy_s(d, 4, (const wchar_t *)v_s), 0); CHECK(d[0] == 0x4e2d && d[1] == 0xfffd && d[2] == 0xe9 && d[3] == 0); }

int main(void) { tc_begin(); TC_RUN(copy_simple); TC_RUN(exact_fit); TC_RUN(empty_src); TC_RUN(non_ascii_wchar_preserved); return tc_end(); }
