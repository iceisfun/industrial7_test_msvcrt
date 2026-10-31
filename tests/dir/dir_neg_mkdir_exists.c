/* TC
 * id:    dir_neg_mkdir_exists
 * area:  dir
 * kind:  negative
 * funcs: _mkdir _rmdir
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mkdir: "EEXIST: Directory not created because dirname is the name of an existing file, directory, or device"
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile d = "tc_dir_neg_mkdir_exists_d";
static const char * volatile f = "tc_dir_neg_mkdir_exists_f";

TC_CASE(existing_dir)
{
    CHECK_EQ_I(_mkdir(d), 0);
    errno = 0;
    CHECK_EQ_I(_mkdir(d), -1);
    CHECK_EQ_I(errno, EEXIST);
    {
        DWORD a = GetFileAttributesA(d);
        CHECK(a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY));
    }
    CHECK_EQ_I(_rmdir(d), 0);
}
TC_CASE(existing_file)
{
    HANDLE h = CreateFileA(f, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    errno = 0;
    CHECK_EQ_I(_mkdir(f), -1);
    CHECK_EQ_I(errno, EEXIST);
    {
        DWORD a = GetFileAttributesA(f);
        CHECK(a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY));
    }
    DeleteFileA(f);
}
int main(void)
{
    tc_begin();
    RemoveDirectoryA("tc_dir_neg_mkdir_exists_d"); DeleteFileA("tc_dir_neg_mkdir_exists_f");
    TC_RUN(existing_dir); TC_RUN(existing_file);
    return tc_end();
}
