/* TC
 * id:    sec_neg_getenv_s_null_name_probe
 * area:  secure_crt
 * funcs: getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs getenv_s
 * note:  Observed on Win10 22H2 19045 ucrtbase: getenv_s(&n, buf, 8, NULL) returns 0 without calling the invalid-parameter
 *        handler (the guess EINVAL + handler was wrong). Deviates from C11 K.3.6.2.1p2 (name == NULL is a runtime-constraint
 *        violation, must return nonzero).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

TC_CASE(null_name_returns_0) { char b[8]; size_t n = 99; int r; b[0] = 'Q'; errno = 0; tc_iph_count = 0; r = getenv_s(&n, b, 8, NULL); CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, 0); }   /* observed: NULL name is not an error */
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_name_returns_0); return tc_end(); }
