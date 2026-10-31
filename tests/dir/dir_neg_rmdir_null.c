/* TC
 * id:    dir_neg_rmdir_null
 * area:  dir
 * kind:  negative
 * funcs: _rmdir _wrmdir
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation" (secure CRT, VS2005+: NULL pointer -> invalid parameter handler, errno EINVAL)
 * note:  Probes that _rmdir(NULL)/_wrmdir(NULL) route through the invalid parameter handler with -1/EINVAL. Not stated in the _rmdir docs table.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile np = NULL;
static const wchar_t * volatile wnp = NULL;

TC_CASE(rmdir_null)  { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_rmdir(np), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wrmdir_null) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wrmdir(wnp), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(rmdir_null); TC_RUN(wrmdir_null);
    return tc_end();
}
