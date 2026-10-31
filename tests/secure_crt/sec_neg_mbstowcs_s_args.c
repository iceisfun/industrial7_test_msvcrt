/* TC
 * id:    sec_neg_mbstowcs_s_args
 * area:  secure_crt
 * funcs: mbstowcs_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs mbstowcs_s error table
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
TC_CASE(null_src_EINVAL) { wchar_t b[8]; size_t n = 9; tc_iph_install(); tc_iph_count = 0; v_sz = 8; CHECK_EQ_I(mbstowcs_s(&n, b, v_sz, NULL, 4), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_dest_nonzero_size_EINVAL) { size_t n = 9; tc_iph_install(); tc_iph_count = 0; v_sz = 8; CHECK_EQ_I(mbstowcs_s(&n, NULL, v_sz, "abc", 4), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(nonnull_dest_size_zero_EINVAL) { wchar_t b[8]; size_t n = 9; tc_iph_install(); tc_iph_count = 0; v_sz = 0; CHECK_EQ_I(mbstowcs_s(&n, b, v_sz, "abc", 4), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(too_small_ERANGE) { wchar_t b[4]; size_t n = 9; int i; tc_iph_install(); v_sz = 4; for (i = 0; i < 4; i++) b[i] = L'x';
    CHECK_EQ_I(mbstowcs_s(&n, b, v_sz, "abcdefgh", 8), ERANGE); }
TC_CASE(handler_counts) { wchar_t b[8]; size_t n = 9; tc_iph_install(); tc_iph_count = 0; v_sz = 8;
    mbstowcs_s(&n, b, v_sz, NULL, 4); CHECK(tc_iph_count >= 1); mbstowcs_s(&n, NULL, v_sz, "a", 1); CHECK(tc_iph_count >= 2);
    v_sz = 0; mbstowcs_s(&n, b, v_sz, "a", 1); CHECK(tc_iph_count >= 3); }
int main(void) { tc_begin(); TC_RUN(null_src_EINVAL); TC_RUN(null_dest_nonzero_size_EINVAL); TC_RUN(nonnull_dest_size_zero_EINVAL);
    TC_RUN(too_small_ERANGE); TC_RUN(handler_counts); return tc_end(); }
