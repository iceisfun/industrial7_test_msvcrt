/* TC
 * id:    sec_neg_putenv_s_invalid
 * area:  secure_crt
 * funcs: _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _putenv_s (EINVAL)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_CASE(null_name_einval) { tc_iph_count = 0; CHECK_EQ_I(_putenv_s(NULL, "v"), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_value_einval) { tc_iph_count = 0; CHECK_EQ_I(_putenv_s("TC_SEC_VAR3", NULL), EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(null_name_einval); TC_RUN(null_value_einval); return tc_end(); }
