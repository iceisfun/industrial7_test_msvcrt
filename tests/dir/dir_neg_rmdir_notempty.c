/* TC
 * id:    dir_neg_rmdir_notempty
 * area:  dir
 * kind:  negative
 * funcs: _mkdir _rmdir
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _rmdir: "ENOTEMPTY: Given path is not a directory, the directory is not empty, or the directory is either the current working directory or the root directory"; "ENOENT: Path not found"
 * note:  The errno for rmdir of a regular file is not asserted (docs say ENOTEMPTY-group / varies), only the -1 return.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile d = "tc_dir_neg_rmdir_notempty_d";
static const char * volatile inner = "tc_dir_neg_rmdir_notempty_d\\f.tmp";
static const char * volatile f = "tc_dir_neg_rmdir_notempty_f.tmp";
static const char * volatile missing = "tc_dir_neg_rmdir_notempty_missing";

static void mkfile(const char *p)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}
TC_CASE(not_empty)
{
    CHECK_EQ_I(_mkdir(d), 0);
    mkfile(inner);
    errno = 0;
    CHECK_EQ_I(_rmdir(d), -1);
    CHECK_EQ_I(errno, ENOTEMPTY);
    {
        DWORD a = GetFileAttributesA(d);
        CHECK(a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY));
    }
    CHECK(GetFileAttributesA(inner) != INVALID_FILE_ATTRIBUTES);
    DeleteFileA(inner);
    CHECK_EQ_I(_rmdir(d), 0);
}
TC_CASE(missing_dir)
{
    errno = 0;
    CHECK_EQ_I(_rmdir(missing), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(regular_file)
{
    mkfile(f);
    CHECK(GetFileAttributesA(f) != INVALID_FILE_ATTRIBUTES);
    CHECK_EQ_I(_rmdir(f), -1);
    CHECK(GetFileAttributesA(f) != INVALID_FILE_ATTRIBUTES);
    DeleteFileA(f);
}
int main(void)
{
    tc_begin();
    DeleteFileA("tc_dir_neg_rmdir_notempty_d\\f.tmp"); RemoveDirectoryA("tc_dir_neg_rmdir_notempty_d"); DeleteFileA("tc_dir_neg_rmdir_notempty_f.tmp");
    TC_RUN(not_empty); TC_RUN(missing_dir); TC_RUN(regular_file);
    return tc_end();
}
