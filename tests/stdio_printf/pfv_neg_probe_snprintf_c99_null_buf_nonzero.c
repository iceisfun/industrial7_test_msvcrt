/* TC
 * id:    pfv_neg_probe_snprintf_c99_null_buf_nonzero
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.5 (NULL buffer with n>0 is UB)
 * note:  PROBING: snprintf(NULL, 8, "abc") on ucrtbase: guess is -1/EINVAL/iph 1; real Windows may instead return 3 with no handler call.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_buf_n8) { char *volatile p = NULL; int n; tc_iph_count = 0; errno = 0; n = snprintf(p, 8, "abc"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf_n8); return tc_end(); }
