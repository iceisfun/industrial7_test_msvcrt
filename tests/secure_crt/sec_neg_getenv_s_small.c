/* TC
 * id:    sec_neg_getenv_s_small
 * area:  secure_crt
 * funcs: getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs getenv_s (ERANGE, EINVAL)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
extern int __cdecl _putenv_s(const char *, const char *);
TC_CASE(buffer_too_small_erange_retsize) { char b[3]; size_t n = 99; _putenv_s("TC_SEC_VAR2", "hello");
    CHECK_EQ_I(getenv_s(&n, b, 3, "TC_SEC_VAR2"), ERANGE); CHECK_EQ_U(n, 6); }
TC_CASE(null_retval_einval) { char b[8]; tc_iph_count = 0; CHECK_EQ_I(getenv_s(NULL, b, 8, "TC_SEC_VAR2"), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_buffer_nonzero_size_einval) { size_t n = 99; tc_iph_count = 0; CHECK_EQ_I(getenv_s(&n, NULL, 8, "TC_SEC_VAR2"), EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(buffer_too_small_erange_retsize); TC_RUN(null_retval_einval); TC_RUN(null_buffer_nonzero_size_einval); _putenv_s("TC_SEC_VAR2", ""); return tc_end(); }
