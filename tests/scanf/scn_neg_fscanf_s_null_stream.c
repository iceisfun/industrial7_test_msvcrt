/* TC
 * id:    scn_neg_fscanf_s_null_stream
 * area:  scanf
 * kind:  negative
 * funcs: fscanf_s fwscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fscanf_s: if stream or format is NULL the invalid parameter handler is invoked; function returns EOF and sets errno to EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE *volatile nfile;
TC_CASE(null_stream) { int n; char a[4]; tc_iph_install(); tc_iph_count = 0; errno = 0; n = fscanf_s(nfile, "%s", a, (unsigned)4); CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(wide_null_stream) { int n; wchar_t a[4]; tc_iph_install(); tc_iph_count = 0; errno = 0; n = fwscanf_s(nfile, L"%s", a, (unsigned)4); CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); TC_RUN(null_stream); TC_RUN(wide_null_stream); return tc_end(); }
