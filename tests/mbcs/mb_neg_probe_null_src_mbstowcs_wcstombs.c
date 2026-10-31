/* TC
 * id:    mb_neg_probe_null_src_mbstowcs_wcstombs
 * area:  mbcs
 * funcs: mbstowcs wcstombs
 * procs: 
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs mbstowcs, wcstombs; Parameter validation
 * note:  PROBE: mbstowcs(dst, NULL, 8) and wcstombs(dst, NULL, 8) with non-NULL dst: expect -1, errno EINVAL, invalid-parameter handler invoked.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

TC_CASE(mbstowcs_null_src)
{
    wchar_t w[8]; size_t n;
    tc_iph_count = 0; errno = 0;
    n = mbstowcs(w, NULL, 8);
    CHECK_EQ_I((long long)n, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(wcstombs_null_src)
{
    char b[8]; size_t n;
    tc_iph_count = 0; errno = 0;
    n = wcstombs(b, NULL, 8);
    CHECK_EQ_I((long long)n, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(mbstowcs_null_src); TC_RUN(wcstombs_null_src);
    return tc_end();
}
