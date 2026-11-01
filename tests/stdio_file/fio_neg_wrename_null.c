/* TC
 * id:    fio_neg_wrename_null
 * area:  stdio_file
 * funcs: _wrename
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs rename, _wrename
 * note:  PROBE: _wrename(NULL,x)/_wrename(x,NULL) returns nonzero, errno EINVAL, invalid parameter handler called once. Validation is not explicitly documented on the rename page.
 *        TRIAGE win10, class A): ucrtbase 10.0.19045 _wrename with a NULL argument returns nonzero, errno ENOENT (2), no handler call (observed got=2, iph 0).
 *        Old EINVAL+handler expectation kept for non-UCRT targets (unverified).
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
#if TC_AT_LEAST(140)
    CHECK(r != 0); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count - n0, 0);   /* observed ucrtbase 10.0.19045 */
#else
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
#endif
}
TC_CASE(null_new)
{
    int n0 = tc_iph_count, r; errno = 0;
    r = _wrename(ok, nul);
#if TC_AT_LEAST(140)
    CHECK(r != 0); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count - n0, 0);   /* observed ucrtbase 10.0.19045 */
#else
    CHECK(r != 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
#endif
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_old); TC_RUN(null_new);
    return tc_end();
}
