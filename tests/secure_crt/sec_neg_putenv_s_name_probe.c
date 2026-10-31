/* TC
 * id:    sec_neg_putenv_s_name_probe
 * area:  secure_crt
 * funcs: _putenv_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _putenv_s (name constraints not tabulated)
 * note:  PROBE: name containing '=' or empty name believed to return EINVAL (CRT env block cannot represent them)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
TC_CASE(name_contains_equals_EINVAL) { tc_iph_install(); CHECK_EQ_I(_putenv_s("TC_SEC=NEG4", "v"), EINVAL); }
TC_CASE(empty_name_EINVAL) { tc_iph_install(); CHECK_EQ_I(_putenv_s("", "v"), EINVAL); }
int main(void) { tc_begin(); TC_RUN(name_contains_equals_EINVAL); TC_RUN(empty_name_EINVAL); return tc_end(); }
