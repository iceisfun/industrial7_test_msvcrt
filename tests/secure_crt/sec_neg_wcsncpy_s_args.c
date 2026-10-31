/* TC
 * id:    sec_neg_wcsncpy_s_args
 * area:  secure_crt
 * funcs: wcsncpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s, wcsncpy_s (error conditions table)
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile const wchar_t *v_s;
static volatile size_t v_n;

TC_CASE(null_dest_EINVAL) { v_s = L"abc"; v_n = 3; RESET(); CHECK_EQ_I(wcsncpy_s(NULL, 8, (const wchar_t *)v_s, v_n), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_src_EINVAL) { wchar_t d[4] = {7, 7, 7, 0}; v_s = NULL; v_n = 3; RESET();
    CHECK_EQ_I(wcsncpy_s(d, 4, (const wchar_t *)v_s, v_n), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(d[0], 0); }
TC_CASE(count_ge_size_ERANGE_dest_emptied) { wchar_t d[4] = {7, 7, 7, 7}; v_s = L"abcdefgh"; v_n = 8; RESET();
    CHECK_EQ_I(wcsncpy_s(d, 4, (const wchar_t *)v_s, v_n), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(handler_counts) { wchar_t d[4]; v_s = L"abcdefgh"; v_n = 8; RESET();
    wcsncpy_s(NULL, 4, (const wchar_t *)v_s, v_n); wcsncpy_s(d, 0, (const wchar_t *)v_s, v_n);
    wcsncpy_s(d, 4, NULL, v_n); wcsncpy_s(d, 4, (const wchar_t *)v_s, v_n);
    CHECK_EQ_I(tc_iph_count, 4); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_dest_EINVAL); TC_RUN(null_src_EINVAL); TC_RUN(count_ge_size_ERANGE_dest_emptied); TC_RUN(handler_counts); return tc_end(); }
