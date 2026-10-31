/* TC
 * id:    sec_mbstowcs_s_basic
 * area:  secure_crt
 * funcs: mbstowcs_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs mbstowcs_s
 * note:  C locale
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

static volatile size_t v_sz = 32;
TC_CASE(convert_ascii_retval_includes_nul) { wchar_t b[32]; size_t n = 99;
    CHECK_EQ_I(mbstowcs_s(&n, b, v_sz, "hello", 31), 0); CHECK_WSTR(b, L"hello"); CHECK_EQ_U(n, 6); }
TC_CASE(null_dest_size_zero_queries_size) { size_t n = 99; CHECK_EQ_I(mbstowcs_s(&n, NULL, 0, "hello", 31), 0); CHECK_EQ_U(n, 6); }
TC_CASE(empty_string) { wchar_t b[4]; size_t n = 99; b[0] = L'x'; CHECK_EQ_I(mbstowcs_s(&n, b, 4, "", 3), 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_U(n, 1); }
TC_CASE(exact_fit) { wchar_t b[6]; size_t n = 99; CHECK_EQ_I(mbstowcs_s(&n, b, 6, "hello", 5), 0); CHECK_WSTR(b, L"hello"); CHECK_EQ_U(n, 6); }
int main(void) { tc_begin(); TC_RUN(convert_ascii_retval_includes_nul); TC_RUN(null_dest_size_zero_queries_size);
    TC_RUN(empty_string); TC_RUN(exact_fit); return tc_end(); }
