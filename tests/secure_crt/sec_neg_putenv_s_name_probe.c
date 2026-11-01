/* TC
 * id:    sec_neg_putenv_s_name_probe
 * area:  secure_crt
 * funcs: _putenv_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _putenv_s (name constraints not tabulated)
 * note:  OBSERVED on Win10 22H2 (19045): _putenv_s("","v") returns EINVAL; _putenv_s("TC_SEC=NEG4","v") returns 0 and sets
 *        TC_SEC to "NEG4=v" (no EINVAL). The earlier expectation that an '=' in the name is rejected was wrong. Stable.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
TC_CASE(name_contains_equals_accepted) { tc_iph_install(); CHECK_EQ_I(_putenv_s("TC_SEC=NEG4", "v"), 0); CHECK_STR(getenv("TC_SEC"), "NEG4=v"); _putenv_s("TC_SEC", ""); }   /* observed */
TC_CASE(empty_name_EINVAL) { tc_iph_install(); CHECK_EQ_I(_putenv_s("", "v"), EINVAL); }
int main(void) { tc_begin(); TC_RUN(name_contains_equals_accepted); TC_RUN(empty_name_EINVAL); return tc_end(); }
