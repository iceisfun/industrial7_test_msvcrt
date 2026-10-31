/* TC
 * id:    sec_neg_wcscpy_s_args
 * area:  secure_crt
 * funcs: wcscpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s, wcscpy_s (error conditions table; sizes in wchar_t)
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile const wchar_t *v_s;
static wchar_t *volatile v_d;
static volatile size_t v_sz;

TC_CASE(null_dest_EINVAL) { v_s = L"abc"; v_d = NULL; v_sz = 4; RESET();
    CHECK_EQ_I(wcscpy_s(v_d, v_sz, (const wchar_t *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_src_EINVAL_dest_emptied) { wchar_t d[4] = {7, 7, 7, 0}; v_s = NULL; RESET();
    CHECK_EQ_I(wcscpy_s(d, 4, (const wchar_t *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(d[0], 0); }
TC_CASE(too_small_ERANGE_dest_emptied) { wchar_t d[4] = {7, 7, 7, 7}; v_s = L"abcd"; RESET();
    CHECK_EQ_I(wcscpy_s(d, 4, (const wchar_t *)v_s), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(handler_counts) { wchar_t d[4]; v_s = L"abcd"; RESET();
    wcscpy_s(d, 4, (const wchar_t *)v_s); wcscpy_s(NULL, 4, (const wchar_t *)v_s); wcscpy_s(d, 4, NULL);
    CHECK_EQ_I(tc_iph_count, 3); RESET(); v_s = L"abc"; CHECK_EQ_I(wcscpy_s(d, 4, (const wchar_t *)v_s), 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_dest_EINVAL); TC_RUN(null_src_EINVAL_dest_emptied); TC_RUN(too_small_ERANGE_dest_emptied); TC_RUN(handler_counts); return tc_end(); }
