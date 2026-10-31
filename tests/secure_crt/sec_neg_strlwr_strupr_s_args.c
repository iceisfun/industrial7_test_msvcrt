/* TC
 * id:    sec_neg_strlwr_strupr_s_args
 * area:  secure_crt
 * funcs: _strlwr_s _strupr_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strlwr_s, _strupr_s (EINVAL)
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
TC_CASE(null_buffer_EINVAL) { tc_iph_install(); tc_iph_count = 0; v_sz = 4; CHECK_EQ_I(_strlwr_s(NULL, v_sz), EINVAL); CHECK_EQ_I(_strupr_s(NULL, v_sz), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(size_zero_EINVAL) { char b[4] = "ab"; tc_iph_install(); v_sz = 0; CHECK_EQ_I(_strlwr_s(b, v_sz), EINVAL); CHECK_EQ_I(_strupr_s(b, v_sz), EINVAL); }
TC_CASE(not_terminated_within_size_EINVAL_lwr) { char b[4] = {'A','B','C','D'}; tc_iph_install(); v_sz = 4; CHECK_EQ_I(_strlwr_s(b, v_sz), EINVAL); }
TC_CASE(not_terminated_within_size_EINVAL_upr) { char b[4] = {'a','b','c','d'}; tc_iph_install(); v_sz = 4; CHECK_EQ_I(_strupr_s(b, v_sz), EINVAL); }
TC_CASE(handler_counts) { char b[4] = {'a','b','c','d'}; tc_iph_install(); tc_iph_count = 0; v_sz = 4;
    _strlwr_s(NULL, v_sz); CHECK(tc_iph_count >= 1); _strupr_s(b, v_sz); CHECK(tc_iph_count >= 2); v_sz = 0; _strlwr_s(b, v_sz); CHECK(tc_iph_count >= 3); }
int main(void) { tc_begin(); TC_RUN(null_buffer_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(not_terminated_within_size_EINVAL_lwr);
    TC_RUN(not_terminated_within_size_EINVAL_upr); TC_RUN(handler_counts); return tc_end(); }
