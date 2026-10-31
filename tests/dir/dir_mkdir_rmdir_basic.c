/* TC
 * id:    dir_mkdir_rmdir_basic
 * area:  dir
 * funcs: _mkdir _rmdir
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mkdir, _wmkdir; _rmdir, _wrmdir (return 0 on success)
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static int is_dir(const char *p) { DWORD a = GetFileAttributesA(p); return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY); }
static int gone(const char *p) { return GetFileAttributesA(p) == INVALID_FILE_ATTRIBUTES; }

static const char * volatile d1 = "tc_dir_mkdir_rmdir_basic_d1";
static const char * volatile par = "tc_dir_mkdir_rmdir_basic_p";
static const char * volatile chi = "tc_dir_mkdir_rmdir_basic_p\\c";
static const char * volatile ts1 = "tc_dir_mkdir_rmdir_basic_s1\\";
static const char * volatile ts2 = "tc_dir_mkdir_rmdir_basic_s2/";

TC_CASE(create_remove)
{
    CHECK_EQ_I(_mkdir(d1), 0);
    CHECK(is_dir(d1));
    CHECK_EQ_I(_rmdir(d1), 0);
    CHECK(gone(d1));
}
TC_CASE(nested)
{
    CHECK_EQ_I(_mkdir(par), 0);
    CHECK_EQ_I(_mkdir(chi), 0);
    CHECK(is_dir(chi));
    CHECK_EQ_I(_rmdir(chi), 0);
    CHECK(gone(chi));
    CHECK(is_dir(par));
    CHECK_EQ_I(_rmdir(par), 0);
    CHECK(gone(par));
}
TC_CASE(trailing_sep)
{
    CHECK_EQ_I(_mkdir(ts1), 0);
    CHECK(is_dir("tc_dir_mkdir_rmdir_basic_s1"));
    CHECK_EQ_I(_rmdir(ts1), 0);
    CHECK(gone("tc_dir_mkdir_rmdir_basic_s1"));
    CHECK_EQ_I(_mkdir(ts2), 0);
    CHECK(is_dir("tc_dir_mkdir_rmdir_basic_s2"));
    CHECK_EQ_I(_rmdir(ts2), 0);
    CHECK(gone("tc_dir_mkdir_rmdir_basic_s2"));
}
int main(void)
{
    tc_begin();
    RemoveDirectoryA("tc_dir_mkdir_rmdir_basic_p\\c"); RemoveDirectoryA("tc_dir_mkdir_rmdir_basic_p");
    RemoveDirectoryA("tc_dir_mkdir_rmdir_basic_d1"); RemoveDirectoryA("tc_dir_mkdir_rmdir_basic_s1"); RemoveDirectoryA("tc_dir_mkdir_rmdir_basic_s2");
    TC_RUN(create_remove); TC_RUN(nested); TC_RUN(trailing_sep);
    return tc_end();
}
