/* TC
 * id:    fio_neg_wrename_args
 * area:  stdio_file
 * funcs: _wrename
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs rename, _wrename (errno ENOENT)
 * note:  missing source gives nonzero + ENOENT. NULL-name validation is in fio_neg_wrename_null.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static const wchar_t * volatile ok = L"tc_fio_neg_wrename_args_ok.tmp";
static const wchar_t * volatile miss = L"tc_fio_neg_wrename_args_missing.tmp";

TC_CASE(missing_src)
{
    int r; errno = 0;
    DeleteFileW(miss);
    r = _wrename(miss, ok);
    CHECK(r != 0); CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_U(GetFileAttributesW(ok), INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    TC_RUN(missing_src);
    DeleteFileW(ok);
    return tc_end();
}
