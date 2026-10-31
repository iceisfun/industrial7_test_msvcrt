/* TC
 * id:    scn_neg_sscanf_s_null_args
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sscanf_s: if str or format is NULL the invalid parameter handler is invoked; if execution continues the function returns EOF and sets errno to EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile nf;
static const char *volatile src;
TC_CASE(null_format) { int n, x = 0; tc_iph_install(); tc_iph_count = 0; errno = 0; src = "12"; n = sscanf_s(src, nf, &x); CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_string) { int n, x = 0; tc_iph_install(); tc_iph_count = 0; errno = 0; nf = "%d"; src = NULL; n = sscanf_s(src, nf, &x); CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); TC_RUN(null_format); TC_RUN(null_string); return tc_end(); }
