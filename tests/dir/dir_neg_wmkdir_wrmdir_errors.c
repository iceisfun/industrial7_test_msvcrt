/* TC
 * id:    dir_neg_wmkdir_wrmdir_errors
 * area:  dir
 * kind:  negative
 * funcs: _wmkdir _wrmdir
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mkdir, _wmkdir (EEXIST, ENOENT), _rmdir, _wrmdir (ENOTEMPTY, ENOENT, EACCES)
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

#define D L"tc_dir_neg_wmw_d"
static const wchar_t *volatile v_d = D;
static const wchar_t *volatile v_noparent = L"tc_dir_neg_wmw_nopar\\child";
static const wchar_t *volatile v_missing = L"tc_dir_neg_wmw_missing";

static void cleanup(void) { DeleteFileW(D L"\\f.tmp"); RemoveDirectoryW(D); }
static int exists(const wchar_t *p) { return GetFileAttributesW(p) != INVALID_FILE_ATTRIBUTES; }

TC_CASE(mkdir_exists)
{
    int r; cleanup();
    CHECK_EQ_I(_wmkdir(v_d), 0);
    errno = 0; r = _wmkdir(v_d);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EEXIST);
    CHECK(exists(D));
}
TC_CASE(mkdir_missing_parent)
{
    int r; errno = 0; r = _wmkdir(v_noparent);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
    CHECK(!exists(L"tc_dir_neg_wmw_nopar"));
}
TC_CASE(rmdir_notempty)
{
    int r; HANDLE h;
    cleanup(); _wmkdir(v_d);
    h = CreateFileW(D L"\\f.tmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    errno = 0; r = _wrmdir(v_d);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOTEMPTY);
    CHECK(exists(D));
    DeleteFileW(D L"\\f.tmp");
    CHECK_EQ_I(_wrmdir(v_d), 0);
    CHECK(!exists(D));
}
TC_CASE(rmdir_missing)
{
    int r; errno = 0; r = _wrmdir(v_missing);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}

int main(void)
{
    tc_begin(); cleanup();
    TC_RUN(mkdir_exists); TC_RUN(mkdir_missing_parent); TC_RUN(rmdir_notempty); TC_RUN(rmdir_missing);
    cleanup();
    return tc_end();
}
