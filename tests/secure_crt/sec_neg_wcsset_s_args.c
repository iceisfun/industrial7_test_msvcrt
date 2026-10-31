/* TC
 * id:    sec_neg_wcsset_s_args
 * area:  secure_crt
 * funcs: _wcsset_s _wcsnset_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strset_s, _wcsset_s, _wcsnset_s
 * note:  kind negative
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static void ini(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
TC_CASE(wcsset_null_EINVAL) { wchar_t *volatile p = NULL; ini(); CHECK_EQ_I(_wcsset_s(p, 8, L'x'), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcsset_unterminated_EINVAL) { wchar_t b[4] = { L'a', L'b', L'c', L'd' }; volatile size_t n = 4; ini(); CHECK_EQ_I(_wcsset_s(b, n, L'x'), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcsnset_null_EINVAL) { wchar_t *volatile p = NULL; ini(); CHECK_EQ_I(_wcsnset_s(p, 8, L'x', 2), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcsnset_size_zero_EINVAL) { wchar_t b[4] = L"ab"; volatile size_t n = 0; ini(); CHECK_EQ_I(_wcsnset_s(b, n, L'x', 1), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); TC_RUN(wcsset_null_EINVAL); TC_RUN(wcsset_unterminated_EINVAL); TC_RUN(wcsnset_null_EINVAL); TC_RUN(wcsnset_size_zero_EINVAL); return tc_end(); }
