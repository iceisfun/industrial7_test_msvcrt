/* TC
 * id:    fio_neg_rename_open_file
 * area:  stdio_file
 * funcs: rename fopen fclose
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs rename
 * note:  rename of a file held open by fopen fails: CRT opens without FILE_SHARE_DELETE, so MoveFile gives a sharing violation mapped to EACCES. Not explicitly documented.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile fa = "tc_fio_neg_rename_open_file_a.tmp";
static const char * volatile fb = "tc_fio_neg_rename_open_file_b.tmp";

TC_CASE(rename_open_fails)
{
    FILE *f; int r;
    f = fopen(fa, "wb"); CHECK(f != NULL); if (!f) return;
    fputc('x', f);
    errno = 0;
    r = rename(fa, fb);
    CHECK(r != 0);
    CHECK_EQ_I(errno, EACCES);
    CHECK_EQ_U(GetFileAttributesA(fb), INVALID_FILE_ATTRIBUTES);
    CHECK(GetFileAttributesA(fa) != INVALID_FILE_ATTRIBUTES);
    fclose(f);
}
TC_CASE(rename_after_close)
{
    FILE *f;
    DeleteFileA(fa); DeleteFileA(fb);
    f = fopen(fa, "wb"); CHECK(f != NULL); if (!f) return;
    fclose(f);
    CHECK_EQ_I(rename(fa, fb), 0);
    CHECK_EQ_U(GetFileAttributesA(fa), INVALID_FILE_ATTRIBUTES);
    CHECK(GetFileAttributesA(fb) != INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fa); DeleteFileA(fb);
    TC_RUN(rename_open_fails); TC_RUN(rename_after_close);
    DeleteFileA(fa); DeleteFileA(fb);
    return tc_end();
}
