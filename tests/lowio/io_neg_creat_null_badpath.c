/* TC
 * id:    io_neg_creat_null_badpath
 * area:  lowio
 * funcs: _creat _wcreat
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _creat/_wcreat: NULL filename invokes invalid parameter handler, EINVAL
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static const char *volatile v_null = NULL;
static const wchar_t *volatile v_wnull = NULL;
static char bad[] = "tc_io_nodir_x\\f";

TC_CASE(creat_null)
{
    int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _creat(v_null, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wcreat_null)
{
    int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _wcreat(v_wnull, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(creat_missing_dir)
{
    int r;
    tc_iph_install();
    errno = 0;
    r = _creat(bad, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
    if (r >= 0) _close(r);
}

int main(void) { tc_begin(); TC_RUN(creat_null); TC_RUN(wcreat_null); TC_RUN(creat_missing_dir); return tc_end(); }
