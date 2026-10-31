/* TC
 * id:    pfi_neg_sprintf_null_buffer
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf: invalid parameter handler
 * note:  msvcrt.dll excluded (no invalid-parameter handler).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile f = "%d";
TC_CASE(null_buf) { char *volatile p = NULL; int n; tc_iph_install(); tc_iph_count = 0; errno = 0; n = sprintf(p, f, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); TC_RUN(null_buf); return tc_end(); }
