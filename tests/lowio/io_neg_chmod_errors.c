/* TC
 * id:    io_neg_chmod_errors
 * area:  lowio
 * funcs: _chmod _wchmod _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _chmod, _wchmod (return value, errno ENOENT); Parameter validation (NULL filename -> EINVAL)
 * note:  Out-of-range pmode bits are probed separately in io_chmod_badmode_probe.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(missing_file)
{
    volatile const char *nm = "tc_io_neg_chmod_missing_" TC_S(TC_CRT_VER) ".tmp"; int r;
    DeleteFileA((const char *)nm);
    errno = 0;
    r = _chmod((const char *)nm, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wmissing_file)
{
    static const wchar_t *volatile nm = L"tc_io_neg_chmod_wmissing_" TC_S(TC_CRT_VER) ".tmp"; int r;
    DeleteFileW(nm);
    errno = 0;
    r = _wchmod(nm, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(null_path)
{
    volatile const char *nm = NULL; int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _chmod((const char *)nm, _S_IREAD);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wnull_path)
{
    const wchar_t *volatile nm = NULL; int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _wchmod(nm, _S_IREAD);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(missing_file); TC_RUN(wmissing_file); TC_RUN(null_path); TC_RUN(wnull_path);
    return tc_end();
}
