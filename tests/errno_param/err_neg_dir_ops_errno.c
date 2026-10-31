/* TC
 * id:    err_neg_dir_ops_errno
 * area:  errno_param
 * kind:  negative
 * funcs: _mkdir _rmdir _chdir
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mkdir, _rmdir, _chdir
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#include <direct.h>
#include <io.h>
#include <errno.h>

static const char *volatile v_d = "tc_err_neg_dir_ops_d" TC_S1(TC_CRT_VER) ".tmp";
static const char *volatile v_m = "tc_err_neg_dir_ops_missing" TC_S1(TC_CRT_VER) ".tmp";
static const char *volatile v_f = "tc_err_neg_dir_ops_d" TC_S1(TC_CRT_VER) ".tmp\\f" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(mkdir_existing)
{
    int r; _rmdir(v_d); CHECK_EQ_I(_mkdir(v_d), 0);
    errno = 0; r = _mkdir(v_d); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EEXIST);
    _rmdir(v_d);
}
TC_CASE(rmdir_missing) { int r; errno = 0; r = _rmdir(v_m); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(chdir_missing) { int r; errno = 0; r = _chdir(v_m); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(rmdir_nonempty)
{
    int r; HANDLE h; _rmdir(v_d);
    CHECK_EQ_I(_mkdir(v_d), 0);
    h = CreateFileA(v_f, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE); if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    errno = 0; r = _rmdir(v_d); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOTEMPTY);
    DeleteFileA(v_f); _rmdir(v_d);
}

int main(void)
{
    tc_begin();
    TC_RUN(mkdir_existing); TC_RUN(rmdir_missing); TC_RUN(chdir_missing); TC_RUN(rmdir_nonempty);
    return tc_end();
}
