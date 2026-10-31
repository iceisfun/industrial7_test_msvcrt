/* TC
 * id:    fio_neg_fopen_null_mode
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen: parameter validation, EINVAL
 * note:  NULL mode invokes the invalid parameter handler.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile nul_mode = NULL;
static const char * volatile fname = "tc_fio_neg_fopen_null_mode_a.tmp";

TC_CASE(null_mode)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = fopen(fname, nul_mode);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    if (f) fclose(f);
    CHECK_EQ_U(GetFileAttributesA(fname), INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fname);
    CHECK(tc_iph_install());
    TC_RUN(null_mode);
    DeleteFileA(fname);
    return tc_end();
}
