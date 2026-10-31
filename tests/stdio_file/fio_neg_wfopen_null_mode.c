/* TC
 * id:    fio_neg_wfopen_null_mode
 * area:  stdio_file
 * kind:  negative
 * funcs: _wfopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen, _wfopen (parameter validation: NULL mode -> EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_mode)
{
    FILE *volatile f; const wchar_t *volatile mode = NULL; tc_iph_count = 0; errno = 0;
    f = _wfopen(L"tc_fio_neg_wfopen_x.tmp", mode);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_mode); return tc_end(); }
