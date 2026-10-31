/* TC
 * id:    sec_neg_wcstombs_s_small
 * area:  secure_crt
 * funcs: wcstombs_s mbstowcs_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs_s, mbstowcs_s (ERANGE, EINVAL)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_CASE(wcs_buffer_too_small_erange) { char b[3]; size_t n = 99; CHECK_EQ_I(wcstombs_s(&n, b, 3, L"abcdef", 6), ERANGE); }
TC_CASE(mbs_buffer_too_small_erange) { wchar_t b[3]; size_t n = 99; CHECK_EQ_I(mbstowcs_s(&n, b, 3, "abcdef", 6), ERANGE); }
TC_CASE(null_src_einval) { char b[3]; size_t n = 99; tc_iph_count = 0; CHECK_EQ_I(wcstombs_s(&n, b, 3, NULL, 1), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_dest_nonzero_size_einval) { size_t n = 99; tc_iph_count = 0; CHECK_EQ_I(wcstombs_s(&n, NULL, 5, L"a", 1), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_retval_ok_or_einval_not_crash) { char b[8]; int r = wcstombs_s(NULL, b, 8, L"abc", 3); CHECK(r == 0 || r == EINVAL); }
int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(wcs_buffer_too_small_erange); TC_RUN(mbs_buffer_too_small_erange); TC_RUN(null_src_einval); TC_RUN(null_dest_nonzero_size_einval); TC_RUN(null_retval_ok_or_einval_not_crash); return tc_end(); }
