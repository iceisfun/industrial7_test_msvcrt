/* TC
 * id:    fio_neg_remove_open_file
 * area:  stdio_file
 * funcs: remove fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs remove: file open => EACCES
 * note:  CRT opens files without FILE_SHARE_DELETE, so DeleteFile hits a sharing violation, mapped to EACCES.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile fname = "tc_fio_neg_remove_open_file_a.tmp";

TC_CASE(open_then_remove)
{
    FILE *f; char b[8] = {0};
    f = fopen(fname, "w+");
    CHECK(f != NULL);
    if (!f) return;
    fputs("abc", f); fflush(f);
    errno = 0;
    CHECK_EQ_I(remove(fname), -1);
    CHECK_EQ_I(errno, EACCES);
    rewind(f);
    CHECK_EQ_U(fread(b, 1, 7, f), 3);
    CHECK_STR(b, "abc");
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(remove(fname), 0);
}
int main(void) { tc_begin(); DeleteFileA(fname); TC_RUN(open_then_remove); DeleteFileA(fname); return tc_end(); }
