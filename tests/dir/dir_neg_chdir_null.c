/* TC
 * id:    dir_neg_chdir_null
 * area:  dir
 * kind:  negative
 * funcs: _chdir _wchdir
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation"; _chdir
 * note:  Probes that _chdir(NULL)/_wchdir(NULL) invoke the invalid parameter handler and return -1 with EINVAL, cwd unchanged.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile np = NULL;
static const wchar_t * volatile wnp = NULL;

TC_CASE(chdir_null)
{
    char b1[MAX_PATH], b2[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, b1);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_chdir(np), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    GetCurrentDirectoryA(MAX_PATH, b2);
    CHECK_STR(b2, b1);
}
TC_CASE(wchdir_null)
{
    char b1[MAX_PATH], b2[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, b1);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_wchdir(wnp), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    GetCurrentDirectoryA(MAX_PATH, b2);
    CHECK_STR(b2, b1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(chdir_null); TC_RUN(wchdir_null);
    return tc_end();
}
