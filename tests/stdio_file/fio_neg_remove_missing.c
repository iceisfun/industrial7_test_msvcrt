/* TC
 * id:    fio_neg_remove_missing
 * area:  stdio_file
 * funcs: remove
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs remove; errno ENOENT
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile fname = "tc_fio_neg_remove_missing_a.tmp";

TC_CASE(missing)
{
    DeleteFileA(fname);
    errno = 0;
    CHECK_EQ_I(remove(fname), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(second_remove)
{
    FILE *f = fopen(fname, "w");
    CHECK(f != NULL);
    if (f) fclose(f);
    CHECK_EQ_I(remove(fname), 0);
    errno = 0;
    CHECK_EQ_I(remove(fname), -1);
    CHECK_EQ_I(errno, ENOENT);
}
int main(void) { tc_begin(); TC_RUN(missing); TC_RUN(second_remove); DeleteFileA(fname); return tc_end(); }
