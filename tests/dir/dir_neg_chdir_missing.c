/* TC
 * id:    dir_neg_chdir_missing
 * area:  dir
 * kind:  negative
 * funcs: _chdir _wchdir _getcwd
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _chdir: "ENOENT: Path not found"; returns -1 on failure
 * note:  For a regular file only the -1 return is asserted (errno not asserted).
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile missing = "tc_dir_neg_chdir_missing_nope";
static const wchar_t * volatile wmissing = L"tc_dir_neg_chdir_missing_nope";
static const char * volatile fname = "tc_dir_neg_chdir_missing_f.tmp";

TC_CASE(missing_dir)
{
    char b1[MAX_PATH], b2[MAX_PATH];
    CHECK(_getcwd(b1, MAX_PATH) != NULL);
    errno = 0;
    CHECK_EQ_I(_chdir(missing), -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK(_getcwd(b2, MAX_PATH) != NULL);
    CHECK_STR(b2, b1);
}
TC_CASE(regular_file)
{
    char b1[MAX_PATH], b2[MAX_PATH];
    HANDLE h = CreateFileA(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    CHECK(_getcwd(b1, MAX_PATH) != NULL);
    CHECK_EQ_I(_chdir(fname), -1);
    CHECK(_getcwd(b2, MAX_PATH) != NULL);
    CHECK_STR(b2, b1);
    DeleteFileA(fname);
}
TC_CASE(wide_missing)
{
    errno = 0;
    CHECK_EQ_I(_wchdir(wmissing), -1);
    CHECK_EQ_I(errno, ENOENT);
}
int main(void)
{
    tc_begin();
    DeleteFileA("tc_dir_neg_chdir_missing_f.tmp");
    TC_RUN(missing_dir); TC_RUN(regular_file); TC_RUN(wide_missing);
    return tc_end();
}
