/* TC
 * id:    sec_neg_env_s_name_probe
 * area:  secure_crt
 * funcs: _putenv_s getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs getenv_s, _putenv_s (name validation not spelled out)
 * note:  PROBE: getenv_s with name==NULL returns EINVAL (retval 0) and _putenv_s with an empty name returns EINVAL. The secure CRT validates these parameters; docs do not list them explicitly.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
TC_CASE(getenv_s_null_name) { char b[8]; size_t n = 99; CHECK_EQ_I(getenv_s(&n, b, 8, NULL), EINVAL); }
TC_CASE(putenv_s_empty_name) { CHECK_EQ_I(_putenv_s("", "v"), EINVAL); }
int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(getenv_s_null_name); TC_RUN(putenv_s_empty_name); return tc_end(); }
