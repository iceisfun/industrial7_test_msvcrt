/* TC
 * id:    sec_neg_splitpath_s_small
 * area:  secure_crt
 * funcs: _splitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _splitpath_s: buffer too small -> ERANGE, invalid-parameter handler; NULL path -> EINVAL

 * note:  [observed on ucrtbase 19045 only; the same expectation on the legacy CRTs listed in crt: is [unverified]] OBSERVED on Win10 22H2 (19045): _splitpath_s with a too-small component buffer returns ERANGE WITHOUT calling the invalid-parameter handler; NULL path and a buffer without size still return EINVAL with
 *        exactly one handler call. The earlier "handler called for ERANGE" expectation was wrong. Stable.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
TC_CASE(fname_buffer_too_small)
{
    char dr[3], di[16], fn[3], ex[8]; int r;
    tc_iph_count = 0; errno = 0;
    r = _splitpath_s("C:\\a\\longname.txt", dr, 3, di, 16, fn, 3, ex, 8);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 0);   /* observed: ERANGE is returned without calling the handler */
}
TC_CASE(null_path_einval)
{
    char fn[16]; int r;
    tc_iph_count = 0; errno = 0;
    r = _splitpath_s(NULL, NULL, 0, NULL, 0, fn, 16, NULL, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(buffer_without_size_einval)
{
    char fn[16]; int r;
    tc_iph_count = 0; errno = 0;
    r = _splitpath_s("a.b", NULL, 0, NULL, 0, fn, 0, NULL, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(fname_buffer_too_small); TC_RUN(null_path_einval); TC_RUN(buffer_without_size_einval); return tc_end(); }
