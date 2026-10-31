/* TC
 * id:    sec_neg_getenv_s_null_name_probe
 * area:  secure_crt
 * funcs: getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs getenv_s
 * note:  PROBE: NULL name is not listed in the docs error table; believed EINVAL with the handler invoked.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

TC_CASE(null_name_EINVAL) { char b[8]; size_t n = 99; int r; errno = 0; tc_iph_count = 0; r = getenv_s(&n, b, 8, NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_name_EINVAL); return tc_end(); }
