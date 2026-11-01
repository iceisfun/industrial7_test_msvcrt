/* TC
 * id:    dir_neg_mkdir_null
 * area:  dir
 * kind:  negative
 * funcs: _mkdir _wmkdir
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs "Parameter validation" (secure CRT, VS2005+: NULL pointer -> invalid parameter handler, errno EINVAL)
 * note:  Probes that _mkdir(NULL)/_wmkdir(NULL) route through the invalid parameter handler with -1/EINVAL. Not stated in the _mkdir docs table.
 *        Win10 22H2 19045 (observed): UCRT _mkdir/_wmkdir do not validate the path pointer; NULL goes to CreateDirectory and fails with errno ENOENT, the invalid parameter handler is NOT called.
 *        [unverified] on other CRTs: only ucrtbase 19045 was observed; the expectation is gated with TC_AT_LEAST(140) and the original expectation is kept for older CRTs.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile np = NULL;
static const wchar_t * volatile wnp = NULL;

TC_CASE(mkdir_null)  { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_mkdir(np), -1);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, 0);   /* observed */
#else
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
#endif
}
TC_CASE(wmkdir_null) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wmkdir(wnp), -1);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, 0);   /* observed */
#else
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
#endif
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(mkdir_null); TC_RUN(wmkdir_null);
    return tc_end();
}
