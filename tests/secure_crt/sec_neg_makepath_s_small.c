/* TC
 * id:    sec_neg_makepath_s_small
 * area:  secure_crt
 * funcs: _makepath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _makepath_s (EINVAL, ERANGE)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
static volatile size_t v_sz;
TC_CASE(buffer_too_small_erange) { char b[8]; v_sz = 8; tc_iph_install(); CHECK_EQ_I(_makepath_s(b, v_sz, "C:", "\\dir\\", "name", ".ext"), ERANGE); }
TC_CASE(one_short_erange) { char b[8]; v_sz = 8; tc_iph_install(); CHECK_EQ_I(_makepath_s(b, v_sz, NULL, NULL, "name12", ".t"), ERANGE); }
TC_CASE(null_path_einval) { v_sz = 16; tc_iph_install(); tc_iph_count = 0; CHECK_EQ_I(_makepath_s(NULL, v_sz, NULL, NULL, "n", NULL), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(zero_size_einval) { char b[8]; v_sz = 0; tc_iph_install(); tc_iph_count = 0; CHECK_EQ_I(_makepath_s(b, v_sz, NULL, NULL, "n", NULL), EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(buffer_too_small_erange); TC_RUN(one_short_erange); TC_RUN(null_path_einval); TC_RUN(zero_size_einval); return tc_end(); }
