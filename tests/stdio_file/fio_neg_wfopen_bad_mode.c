/* TC
 * id:    fio_neg_wfopen_bad_mode
 * area:  stdio_file
 * kind:  negative
 * funcs: _wfopen
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fopen, _wfopen (mode strings); the docs do not explicitly list an unknown mode letter as a validation failure
 * note:  An unrecognised first mode character ("x") is rejected with EINVAL and the invalid parameter handler on the
 *        secure-CRT generations (msvcr80+). Split from the NULL-argument tests because only the NULL cases are spelled out in the docs.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(bad_mode)
{
    FILE *volatile f; const wchar_t *volatile mode = L"x"; tc_iph_count = 0; errno = 0;
    f = _wfopen(L"tc_fio_neg_wfopen_bad_mode.tmp", mode);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
    if (f) fclose(f);
    DeleteFileA("tc_fio_neg_wfopen_bad_mode.tmp");
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(bad_mode);
    return tc_end();
}
