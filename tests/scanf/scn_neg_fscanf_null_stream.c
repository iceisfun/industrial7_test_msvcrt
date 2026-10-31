/* TC
 * id:    scn_neg_fscanf_null_stream
 * area:  scanf
 * funcs: fscanf fwscanf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fscanf, fwscanf: NULL stream invokes the invalid parameter handler, returns EOF, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static FILE *volatile vf;

TC_CASE(fscanf_null)
{
    int a = 5, r; vf = NULL;
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    r = fscanf(vf, "%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(a, 5);
}
TC_CASE(fwscanf_null)
{
    int a = 5, r; vf = NULL;
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    r = fwscanf(vf, L"%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(a, 5);
}
int main(void) { tc_begin(); TC_RUN(fscanf_null); TC_RUN(fwscanf_null); return tc_end(); }
