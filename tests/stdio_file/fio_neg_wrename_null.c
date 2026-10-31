/* TC
 * id:    fio_neg_wrename_null
 * area:  stdio_file
 * funcs: _wrename
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs rename, _wrename
 * note:  PROBE: _wrename(NULL,x)/_wrename(x,NULL) returns nonzero, errno EINVAL, invalid parameter handler called once. Validation is not explicitly documented on the rename page.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static const wchar_t * volatile nul = NULL;
static const wchar_t * volatile ok = L"tc_fio_neg_wrename_null_ok.tmp";

TC_CASE(null_old)
{
    int n0 = tc_iph_count, r; errno = 0;
    r = _wrename(nul, ok);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(null_new)
{
    int n0 = tc_iph_count, r; errno = 0;
    r = _wrename(ok, nul);
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_old); TC_RUN(null_new);
    return tc_end();
}
