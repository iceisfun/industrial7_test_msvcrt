/* TC
 * id:    dir_neg_mkdir_null
 * area:  dir
 * kind:  negative
 * funcs: _mkdir _wmkdir
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation" (secure CRT, VS2005+: NULL pointer -> invalid parameter handler, errno EINVAL)
 * note:  Probes that _mkdir(NULL)/_wmkdir(NULL) route through the invalid parameter handler with -1/EINVAL. Not stated in the _mkdir docs table.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile np = NULL;
static const wchar_t * volatile wnp = NULL;

TC_CASE(mkdir_null)  { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_mkdir(np), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wmkdir_null) { tc_iph_count = 0; errno = 0; CHECK_EQ_I(_wmkdir(wnp), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(mkdir_null); TC_RUN(wmkdir_null);
    return tc_end();
}
