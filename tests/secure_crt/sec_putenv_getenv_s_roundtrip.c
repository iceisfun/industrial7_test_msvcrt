/* TC
 * id:    sec_putenv_getenv_s_roundtrip
 * area:  secure_crt
 * funcs: _putenv_s getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv_s, getenv_s
 * note:  unique variable TC_SEC_VAR1
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

static volatile size_t v_sz = 64;
TC_CASE(set_and_get) { char b[64]; size_t n = 99; CHECK_EQ_I(_putenv_s("TC_SEC_VAR1", "hello"), 0); CHECK_EQ_I(getenv_s(&n, b, v_sz, "TC_SEC_VAR1"), 0); CHECK_STR(b, "hello"); }
TC_CASE(retsize_includes_nul) { char b[64]; size_t n = 99; _putenv_s("TC_SEC_VAR1", "hello"); CHECK_EQ_I(getenv_s(&n, b, v_sz, "TC_SEC_VAR1"), 0); CHECK_EQ_U(n, 6); }
TC_CASE(replace_value) { char b[64]; size_t n = 99; _putenv_s("TC_SEC_VAR1", "one"); CHECK_EQ_I(_putenv_s("TC_SEC_VAR1", "twotwo"), 0);
    CHECK_EQ_I(getenv_s(&n, b, v_sz, "TC_SEC_VAR1"), 0); CHECK_STR(b, "twotwo"); CHECK_EQ_U(n, 7); }
TC_CASE(size_query_with_null_buffer_zero_size) { size_t n = 99; _putenv_s("TC_SEC_VAR1", "hello");
    CHECK_EQ_I(getenv_s(&n, NULL, 0, "TC_SEC_VAR1"), 0); CHECK_EQ_U(n, 6); }
TC_CASE(unset_variable_retsize_zero_ret_zero) { char b[16]; size_t n = 99; _putenv_s("TC_SEC_VAR1", "");
    b[0] = 'x'; CHECK_EQ_I(getenv_s(&n, b, sizeof b, "TC_SEC_VAR1"), 0); CHECK_EQ_U(n, 0); CHECK_EQ_I(b[0], 0); }
TC_CASE(empty_value_removes_variable) { char b[16]; size_t n = 99; CHECK_EQ_I(_putenv_s("TC_SEC_VAR1", "abc"), 0);
    CHECK_EQ_I(getenv_s(&n, b, sizeof b, "TC_SEC_VAR1"), 0); CHECK_EQ_U(n, 4);
    CHECK_EQ_I(_putenv_s("TC_SEC_VAR1", ""), 0);
    n = 99; CHECK_EQ_I(getenv_s(&n, b, sizeof b, "TC_SEC_VAR1"), 0); CHECK_EQ_U(n, 0); }
int main(void) { tc_begin(); TC_RUN(set_and_get); TC_RUN(retsize_includes_nul); TC_RUN(replace_value); TC_RUN(size_query_with_null_buffer_zero_size);
    TC_RUN(unset_variable_retsize_zero_ret_zero); TC_RUN(empty_value_removes_variable); return tc_end(); }
