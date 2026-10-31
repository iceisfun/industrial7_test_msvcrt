/* TC
 * id:    dir_neg_rmdir_empty_probe
 * area:  dir
 * kind:  negative
 * funcs: _rmdir _wrmdir _mkdir _chdir _getcwd
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _rmdir (-1 on failure)
 * note:  Probes _rmdir("")/_wrmdir(L"") -> -1 with errno in {ENOENT, EINVAL}, and _rmdir(".") -> -1 with errno in {EACCES, EINVAL, ENOENT}. The "." case is
 *        run inside a freshly created scratch directory so a (non-expected) success cannot remove anything of value;
 *        the cwd must still be valid afterwards.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

#define SCR "tc_dir_neg_rmdir_empty_scr"
static const char *volatile v_empty = "";
static const wchar_t *volatile v_wempty = L"";
static const char *volatile v_dot = ".";

TC_CASE(empty)
{
    int r; errno = 0; r = _rmdir(v_empty);
    CHECK_EQ_I(r, -1); CHECK(errno == ENOENT || errno == EINVAL);
}
TC_CASE(wide_empty)
{
    int r; errno = 0; r = _wrmdir(v_wempty);
    CHECK_EQ_I(r, -1); CHECK(errno == ENOENT || errno == EINVAL);
}
TC_CASE(dot)
{
    char orig[MAX_PATH], now[MAX_PATH]; int r;
    _getcwd(orig, MAX_PATH);
    RemoveDirectoryA(SCR);
    CHECK_EQ_I(_mkdir(SCR), 0);
    if (_chdir(SCR) != 0) { RemoveDirectoryA(SCR); CHECK(0); return; }
    errno = 0; r = _rmdir(v_dot);
    CHECK_EQ_I(r, -1);
    CHECK(errno == EACCES || errno == EINVAL || errno == ENOENT);
    CHECK(_getcwd(now, MAX_PATH) != NULL);
    _chdir(orig);
    RemoveDirectoryA(SCR);
}

int main(void)
{
    tc_begin();
    TC_RUN(empty); TC_RUN(wide_empty); TC_RUN(dot);
    return tc_end();
}
