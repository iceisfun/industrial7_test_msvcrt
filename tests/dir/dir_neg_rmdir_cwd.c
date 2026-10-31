/* TC
 * id:    dir_neg_rmdir_cwd
 * area:  dir
 * kind:  negative
 * funcs: _chdir _rmdir _mkdir _getcwd
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _rmdir: the directory may not be the current working directory
 * note:  Probes that removing the process's current directory fails with -1 on real Windows (documented as an error;
 *        errno not asserted, EACCES/ENOTEMPTY expected). "." and the absolute cwd path are both tried.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static char orig[MAX_PATH];
static const char * volatile d = "tc_dir_neg_rmdir_cwd_d";

TC_CASE(rmdir_cwd)
{
    char cur[MAX_PATH];
    CHECK_EQ_I(_mkdir(d), 0);
    CHECK_EQ_I(_chdir(d), 0);
    CHECK_EQ_I(_rmdir("."), -1);
    CHECK(_getcwd(cur, MAX_PATH) != NULL);
    CHECK_EQ_I(_rmdir(cur), -1);
    CHECK_EQ_I(_chdir(".."), 0);
    {
        DWORD a = GetFileAttributesA(d);
        CHECK(a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY));
    }
    CHECK_EQ_I(_rmdir(d), 0);
}
int main(void)
{
    tc_begin();
    GetCurrentDirectoryA(MAX_PATH, orig);
    RemoveDirectoryA("tc_dir_neg_rmdir_cwd_d");
    TC_RUN(rmdir_cwd);
    SetCurrentDirectoryA(orig);
    RemoveDirectoryA("tc_dir_neg_rmdir_cwd_d");
    return tc_end();
}
