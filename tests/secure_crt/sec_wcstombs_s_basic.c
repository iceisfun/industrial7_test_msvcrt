/* TC
 * id:    sec_wcstombs_s_basic
 * area:  secure_crt
 * funcs: wcstombs_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs wcstombs_s
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
TC_CASE(convert_ascii_retval_includes_nul) { char b[32]; size_t n = 99; memset(b, 'x', sizeof b);
    CHECK_EQ_I(wcstombs_s(&n, b, v_sz, L"hello", 31), 0); CHECK_STR(b, "hello"); CHECK_EQ_U(n, 6); }
TC_CASE(null_dest_size_zero_queries_size) { size_t n = 99; CHECK_EQ_I(wcstombs_s(&n, NULL, 0, L"hello", 31), 0); CHECK_EQ_U(n, 6); }
TC_CASE(empty_string_retval_1) { char b[8]; size_t n = 99; memset(b, 'x', sizeof b);
    CHECK_EQ_I(wcstombs_s(&n, b, sizeof b, L"", 7), 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_U(n, 1); }
TC_CASE(exact_fit) { char b[6]; size_t n = 99; CHECK_EQ_I(wcstombs_s(&n, b, 6, L"hello", 5), 0); CHECK_STR(b, "hello"); CHECK_EQ_U(n, 6); }
int main(void) { tc_begin(); TC_RUN(convert_ascii_retval_includes_nul); TC_RUN(null_dest_size_zero_queries_size);
    TC_RUN(empty_string_retval_1); TC_RUN(exact_fit); return tc_end(); }
