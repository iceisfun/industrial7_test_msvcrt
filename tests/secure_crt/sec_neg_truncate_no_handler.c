/* TC
 * id:    sec_neg_truncate_no_handler
 * area:  secure_crt
 * funcs: strncpy_s strncat_s wcsncpy_s wcstombs_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s, strncat_s, wcstombs_s (_TRUNCATE; STRUNCATE = 80)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
static const wchar_t *volatile v_w = L"abcdef";

TC_CASE(strncpy_s_truncate_returns_80_no_handler) { char d[4]; setup(); CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, _TRUNCATE), STRUNCATE); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(strncat_s_truncate_returns_80_no_handler) { char d[5] = "ab"; setup(); CHECK_EQ_I(strncat_s(d, sizeof d, (const char *)v_s, _TRUNCATE), STRUNCATE); CHECK_STR(d, "abab"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(wcsncpy_s_truncate_no_handler) { wchar_t d[4]; setup(); CHECK_EQ_I(wcsncpy_s(d, 4, v_w, _TRUNCATE), STRUNCATE); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(wcstombs_s_truncate_no_handler) { char d[4]; size_t n = 0; setup(); CHECK_EQ_I(wcstombs_s(&n, d, sizeof d, v_w, _TRUNCATE), STRUNCATE); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(return_value_nonzero) { char d[2]; setup(); CHECK(strncpy_s(d, sizeof d, (const char *)v_s, _TRUNCATE) != 0); CHECK(strncat_s(d, sizeof d, (const char *)v_s, _TRUNCATE) != 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_RUN(strncpy_s_truncate_returns_80_no_handler); TC_RUN(strncat_s_truncate_returns_80_no_handler); TC_RUN(wcsncpy_s_truncate_no_handler); TC_RUN(wcstombs_s_truncate_no_handler); TC_RUN(return_value_nonzero); return tc_end(); }
