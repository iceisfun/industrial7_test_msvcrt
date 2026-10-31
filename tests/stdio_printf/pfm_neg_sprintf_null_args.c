/* TC
 * id:    pfm_neg_sprintf_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l, swprintf: if buffer or format is a null pointer the invalid parameter handler is invoked, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile nullfmt = NULL;
static char * volatile nullbuf = NULL;

TC_CASE(null_format) { char b[16]; int r; b[0] = 'x'; tc_iph_count = 0; errno = 0; r = sprintf(b, nullfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buffer) { int r; tc_iph_count = 0; errno = 0; r = sprintf(nullbuf, "%d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; } TC_RUN(null_format); TC_RUN(null_buffer); return tc_end(); }
