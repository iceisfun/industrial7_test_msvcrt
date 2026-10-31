/* TC
 * id:    err_neg_fopen_missing_file
 * area:  errno_param
 * funcs: fopen __doserrno
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs fopen (errno); ERROR_FILE_NOT_FOUND=2, ERROR_PATH_NOT_FOUND=3
 * note:  errno ENOENT documented; _doserrno values observed on Windows.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

extern unsigned long *__cdecl __doserrno(void);

TC_CASE(missing_file)
{
    FILE *volatile f; errno = 0; *__doserrno() = 0;
    f = fopen("tc_err_neg_fopen_missing_file_nonexistent.tmp", "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_U(*__doserrno(), 2);
    if (f) fclose(f);
}
TC_CASE(missing_dir)
{
    FILE *volatile f; errno = 0; *__doserrno() = 0;
    f = fopen("tc_err_no_such_dir_xyz\\file.tmp", "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_U(*__doserrno(), 3);
    if (f) fclose(f);
}
int main(void) { tc_begin(); TC_RUN(missing_file); TC_RUN(missing_dir); return tc_end(); }
