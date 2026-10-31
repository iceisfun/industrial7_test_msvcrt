/* TC
 * id:    dir_neg_getdcwd_badargs
 * area:  dir
 * funcs: _getdcwd _wgetdcwd
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _getdcwd; Parameter validation
 * note:  ERANGE documented. Bad drive numbers only assert a NULL return; errno / iph behaviour is in dir_neg_getdcwd_baddrive_probe. maxlen==0 with a buffer is probed in dir_neg_getcwd_zero_size.
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_zero, v_two = 2, v_m1 = -1, v_27 = 27;

TC_CASE(small_buffer)
{
    char buf[8]; char *p; tc_iph_install();
    errno = 0; p = _getdcwd(0, buf, v_two);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(wide_small_buffer)
{
    wchar_t buf[8]; wchar_t *p; tc_iph_install();
    errno = 0; p = _wgetdcwd(0, buf, v_two);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(bad_drive_returns_null)
{
    char buf[MAX_PATH]; tc_iph_install();
    CHECK(_getdcwd(v_m1, buf, MAX_PATH) == NULL);
    CHECK(_getdcwd(v_27, buf, MAX_PATH) == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(small_buffer); TC_RUN(wide_small_buffer); TC_RUN(bad_drive_returns_null);
    return tc_end();
}
