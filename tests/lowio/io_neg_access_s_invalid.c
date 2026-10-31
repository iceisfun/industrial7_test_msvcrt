/* TC
 * id:    io_neg_access_s_invalid
 * area:  lowio
 * funcs: _access_s _waccess_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _access_s (EINVAL for NULL path); Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(null_path)
{
    const char *volatile p = NULL;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_access_s((const char *)p, 0), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_wide_path)
{
    const wchar_t *volatile p = NULL;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    CHECK_EQ_I(_waccess_s((const wchar_t *)p, 0), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_path); TC_RUN(null_wide_path); return tc_end(); }
