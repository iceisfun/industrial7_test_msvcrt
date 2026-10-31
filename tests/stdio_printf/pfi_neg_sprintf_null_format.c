/* TC
 * id:    pfi_neg_sprintf_null_format
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf: invalid parameter handler
 * note:  buffer[0] state after failure is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
TC_CASE(null_fmt) { char b[16]; const char *volatile f = NULL; int n; tc_iph_install(); tc_iph_count = 0; errno = 0; b[0] = 'Z'; n = sprintf(b, f); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); TC_RUN(null_fmt); return tc_end(); }
