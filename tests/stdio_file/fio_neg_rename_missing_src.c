/* TC
 * id:    fio_neg_rename_missing_src
 * area:  stdio_file
 * funcs: rename
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs rename: errno ENOENT if old name not found
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile na = "tc_fio_neg_rename_missing_src_a.tmp";
static const char * volatile nb = "tc_fio_neg_rename_missing_src_b.tmp";

TC_CASE(missing)
{
    int r;
    DeleteFileA(na); DeleteFileA(nb);
    errno = 0;
    r = rename(na, nb);
    CHECK(r != 0);
    CHECK_EQ_I(errno, ENOENT);
    CHECK(GetFileAttributesA(nb) == INVALID_FILE_ATTRIBUTES);
}
int main(void) { tc_begin(); TC_RUN(missing); DeleteFileA(nb); return tc_end(); }
