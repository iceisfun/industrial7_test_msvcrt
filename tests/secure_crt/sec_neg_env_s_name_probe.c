/* TC
 * id:    sec_neg_env_s_name_probe
 * area:  secure_crt
 * funcs: _putenv_s getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs getenv_s, _putenv_s (name validation not spelled out)
 * note:  [observed on ucrtbase 19045 only; the same expectation on the legacy CRTs listed in crt: is [unverified]] OBSERVED on Win10 22H2 (19045): getenv_s(&n,buf,8,NULL) returns 0 with *retval 0 and NO handler call (a NULL name is
 *        treated as "variable not found"); _putenv_s("","v") returns EINVAL. The earlier EINVAL expectation for the NULL name
 *        was wrong. Stable.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
TC_CASE(getenv_s_null_name) { char b[8]; size_t n = 99; tc_iph_count = 0; CHECK_EQ_I(getenv_s(&n, b, 8, NULL), 0); CHECK_EQ_I(n, 0); CHECK_EQ_I(tc_iph_count, 0); }   /* observed: NULL name = not found */
TC_CASE(putenv_s_empty_name) { CHECK_EQ_I(_putenv_s("", "v"), EINVAL); }
int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(getenv_s_null_name); TC_RUN(putenv_s_empty_name); return tc_end(); }
