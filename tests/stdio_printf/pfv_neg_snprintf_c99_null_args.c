/* TC
 * id:    pfv_neg_snprintf_c99_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs snprintf (UCRT): NULL format invalid; buffer NULL with count 0 is a valid length query (C11 7.21.6.5)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_fmt) { char b[16]; const char *volatile f = NULL; int n; tc_iph_count = 0; errno = 0; n = snprintf(b, 8, f); CHECK(n < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buf_n0_is_valid) { char *volatile p = NULL; volatile size_t z = 0; int n; tc_iph_count = 0; n = snprintf(p, z, "abc"); CHECK_EQ_I(n, 3); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_fmt); TC_RUN(null_buf_n0_is_valid); return tc_end(); }
