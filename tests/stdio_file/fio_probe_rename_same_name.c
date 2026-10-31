/* TC
 * id:    fio_probe_rename_same_name
 * area:  stdio_file
 * funcs: rename fopen fclose
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs rename
 * note:  PROBE: rename(a,a) on an existing closed file. Result (0 or -1) is not asserted; MoveFile of a file to itself fails with ERROR_ALREADY_EXISTS on some versions. Only asserts that the file still exists with its content.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile fa = "tc_fio_probe_rename_same_name_a.tmp";

TC_CASE(same_name)
{
    FILE *f; volatile int r; char b[8] = {0};
    f = fopen(fa, "wb"); CHECK(f != NULL); if (!f) return;
    fputs("abc", f); fclose(f);
    r = rename(fa, fa);
    CHECK(r == 0 || r == -1 || r != 0);
    f = fopen(fa, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(b, 1, 7, f), 3);
    CHECK_STR(b, "abc");
    fclose(f);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fa);
    TC_RUN(same_name);
    DeleteFileA(fa);
    return tc_end();
}
