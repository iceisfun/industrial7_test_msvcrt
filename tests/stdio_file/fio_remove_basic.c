/* TC
 * id:    fio_remove_basic
 * area:  stdio_file
 * funcs: remove fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.4.1; MS docs remove
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile fname = "tc_fio_remove_basic_a.tmp";
static const char * volatile dname = "tc_fio_remove_basic_d.tmp";
static const char * volatile dfname = "tc_fio_remove_basic_d.tmp\\f.tmp";

static void mkfile(const char *n)
{
    FILE *f = fopen(n, "w");
    CHECK(f != NULL);
    if (f) { fputs("x", f); fclose(f); }
}
TC_CASE(remove_closed)
{
    FILE *f;
    mkfile(fname);
    CHECK_EQ_I(remove(fname), 0);
    errno = 0;
    f = fopen(fname, "r");
    CHECK(f == NULL);
    CHECK_EQ_I(errno, ENOENT);
    if (f) fclose(f);
}
TC_CASE(remove_in_dir)
{
    CHECK(CreateDirectoryA(dname, NULL));
    mkfile(dfname);
    CHECK_EQ_I(remove(dfname), 0);
    CHECK(GetFileAttributesA(dfname) == INVALID_FILE_ATTRIBUTES);
    RemoveDirectoryA(dname);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fname); DeleteFileA(dfname); RemoveDirectoryA(dname);
    TC_RUN(remove_closed); TC_RUN(remove_in_dir);
    DeleteFileA(fname); DeleteFileA(dfname); RemoveDirectoryA(dname);
    return tc_end();
}
