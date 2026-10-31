/* TC
 * id:    io_neg_access_errors
 * area:  lowio
 * funcs: _access _waccess _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _access (ENOENT; EINVAL for NULL path); Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char *volatile missing = "tc_io_neg_access_missing.tmp";
TC_CASE(missing_file)
{
    errno = 0;
    CHECK_EQ_I(_access(missing, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(null_path)
{
    const char *volatile p = NULL;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_access((const char *)p, 0), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_variants)
{
    const wchar_t *volatile p = NULL;
    const wchar_t *volatile nm = L"tc_io_neg_access_missing.tmp";
    CHECK(tc_iph_install());
    errno = 0;
    CHECK_EQ_I(_waccess(nm, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_waccess((const wchar_t *)p, 0), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(missing_file);
    TC_RUN(null_path); TC_RUN(wide_variants);
    return tc_end();
}
