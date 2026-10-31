/* TC
 * id:    dir_neg_chdir_empty_probe
 * area:  dir
 * kind:  negative
 * funcs: _chdir _wchdir _getcwd
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _chdir (-1 on failure)
 * note:  Probes (a) _chdir("") and _wchdir(L"") return -1 with errno ENOENT or EINVAL (the Win32 error from SetCurrentDirectory is
 *        not known to map to one specific errno), (b) a path with the illegal character '<' fails with -1, and that the cwd is unchanged in every case.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char *volatile v_empty = "";
static const wchar_t *volatile v_wempty = L"";
static const char *volatile v_illegal = "a<b";

static int same_cwd(const char *ref) { char b[MAX_PATH]; return _getcwd(b, MAX_PATH) && tc_strequ(b, ref); }

TC_CASE(empty)
{
    char ref[MAX_PATH]; int r;
    _getcwd(ref, MAX_PATH);
    errno = 0; r = _chdir(v_empty);
    CHECK_EQ_I(r, -1); CHECK(errno == ENOENT || errno == EINVAL);
    CHECK(same_cwd(ref));
}
TC_CASE(illegal_char)
{
    char ref[MAX_PATH]; int r;
    _getcwd(ref, MAX_PATH);
    r = _chdir(v_illegal);
    CHECK_EQ_I(r, -1);
    CHECK(same_cwd(ref));
}
TC_CASE(wide_empty)
{
    char ref[MAX_PATH]; int r;
    _getcwd(ref, MAX_PATH);
    errno = 0; r = _wchdir(v_wempty);
    CHECK_EQ_I(r, -1); CHECK(errno == ENOENT || errno == EINVAL);
    CHECK(same_cwd(ref));
}

int main(void)
{
    tc_begin();
    TC_RUN(empty); TC_RUN(illegal_char); TC_RUN(wide_empty);
    return tc_end();
}
