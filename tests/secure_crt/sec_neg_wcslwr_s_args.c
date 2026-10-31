/* TC
 * id:    sec_neg_wcslwr_s_args
 * area:  secure_crt
 * funcs: _wcslwr_s _wcsupr_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strlwr_s, _wcslwr_s
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
TC_CASE(null_EINVAL) { tc_iph_install(); tc_iph_count = 0; v_sz = 4; CHECK_EQ_I(_wcslwr_s(NULL, v_sz), EINVAL); CHECK_EQ_I(_wcsupr_s(NULL, v_sz), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(size_zero_EINVAL) { wchar_t b[4] = L"ab"; tc_iph_install(); v_sz = 0; CHECK_EQ_I(_wcslwr_s(b, v_sz), EINVAL); CHECK_EQ_I(_wcsupr_s(b, v_sz), EINVAL); }
TC_CASE(unterminated_EINVAL) { wchar_t b[4] = {L'A',L'B',L'C',L'D'}, c[4] = {L'a',L'b',L'c',L'd'}; tc_iph_install(); v_sz = 4;
    CHECK_EQ_I(_wcslwr_s(b, v_sz), EINVAL); CHECK_EQ_I(_wcsupr_s(c, v_sz), EINVAL); }
TC_CASE(handler_counts) { wchar_t b[4] = {L'a',L'b',L'c',L'd'}; tc_iph_install(); tc_iph_count = 0; v_sz = 4;
    _wcslwr_s(NULL, v_sz); CHECK(tc_iph_count >= 1); _wcsupr_s(b, v_sz); CHECK(tc_iph_count >= 2); v_sz = 0; _wcslwr_s(b, v_sz); CHECK(tc_iph_count >= 3); }
int main(void) { tc_begin(); TC_RUN(null_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(unterminated_EINVAL); TC_RUN(handler_counts); return tc_end(); }
