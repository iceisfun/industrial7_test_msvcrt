/* TC
 * id:    sec_neg_wcstombs_s_args
 * area:  secure_crt
 * funcs: wcstombs_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs_s error table
 * note:  
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
static volatile size_t v_sz;
TC_CASE(null_src_EINVAL) { char b[8]; size_t n = 9; tc_iph_install(); tc_iph_count = 0; v_sz = sizeof b;
    CHECK_EQ_I(wcstombs_s(&n, b, v_sz, NULL, 4), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_dest_nonzero_size_EINVAL) { size_t n = 9; tc_iph_install(); tc_iph_count = 0; v_sz = 8;
    CHECK_EQ_I(wcstombs_s(&n, NULL, v_sz, L"abc", 4), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(nonnull_dest_size_zero_EINVAL) { char b[8]; size_t n = 9; tc_iph_install(); tc_iph_count = 0; v_sz = 0;
    CHECK_EQ_I(wcstombs_s(&n, b, v_sz, L"abc", 4), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(handler_counts) { char b[8]; size_t n = 9; tc_iph_install(); tc_iph_count = 0; v_sz = 8;
    wcstombs_s(&n, b, v_sz, NULL, 4); CHECK(tc_iph_count >= 1); wcstombs_s(&n, NULL, v_sz, L"a", 1); CHECK(tc_iph_count >= 2);
    v_sz = 0; wcstombs_s(&n, b, v_sz, L"a", 1); CHECK(tc_iph_count >= 3); }
int main(void) { tc_begin(); TC_RUN(null_src_EINVAL); TC_RUN(null_dest_nonzero_size_EINVAL); TC_RUN(nonnull_dest_size_zero_EINVAL); TC_RUN(handler_counts); return tc_end(); }
