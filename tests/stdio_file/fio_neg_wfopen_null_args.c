/* TC
 * id:    fio_neg_wfopen_null_args
 * area:  stdio_file
 * kind:  negative
 * funcs: _wfopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen, _wfopen (parameter validation: NULL filename -> EINVAL + invalid parameter handler); errno ENOENT for a missing file (errno.h docs)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_name)
{
    FILE *volatile f; const wchar_t *volatile name = NULL; tc_iph_count = 0; errno = 0;
    f = _wfopen(name, L"r");
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(missing_file)
{
    FILE *volatile f; const wchar_t *volatile name = L"tc_fio_neg_wfopen_missing.tmp"; errno = 0;
    DeleteFileA("tc_fio_neg_wfopen_missing.tmp");
    f = _wfopen(name, L"r");
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_name); TC_RUN(missing_file);
    return tc_end();
}
