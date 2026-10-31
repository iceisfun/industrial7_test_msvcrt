/* TC
 * id:    dir_neg_getcwd_zero_size
 * area:  dir
 * funcs: _getcwd _wgetcwd
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _getcwd: maxlen <= 0 with non-NULL buffer invokes the invalid parameter handler, errno EINVAL
 * note:  PROBE: MS docs say maxlen <= 0 invokes the invalid parameter handler (EINVAL), but docs also say (NULL,0) allocates, and the
 *        CRT may validate only maxlen < 0, in which case buffer+0 yields NULL/ERANGE with no handler. Real Windows decides.
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_zero;

TC_CASE(narrow_zero)
{
    char buf[16]; char *p; int i;
    for (i = 0; i < 16; i++) buf[i] = 'Z';
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    p = _getcwd(buf, v_zero);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 16; i++) CHECK_EQ_I(buf[i], 'Z');
}
TC_CASE(wide_zero)
{
    wchar_t buf[16]; wchar_t *p; int i;
    for (i = 0; i < 16; i++) buf[i] = L'Z';
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    p = _wgetcwd(buf, v_zero);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 16; i++) CHECK_EQ_I(buf[i], L'Z');
}
int main(void)
{
    tc_begin();
    TC_RUN(narrow_zero); TC_RUN(wide_zero);
    return tc_end();
}
