/* TC
 * id:    gdr_chdir_rmdir_forms
 * area:  dir
 * funcs: _chdir _rmdir _mkdir
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _chdir, _rmdir, _mkdir (Windows path forms: / and \ both accepted)
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define R "tc_gdr_chdir_forms" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_p;
static char saved[MAX_PATH], now[MAX_PATH];

static void getcwdw(char *b) { GetCurrentDirectoryA(MAX_PATH, b); }
static void restore(void) { SetCurrentDirectoryA(saved); }
static void cleanup(void)
{
    restore();
    RemoveDirectoryA(R "\\sub\\in"); RemoveDirectoryA(R "\\sub"); RemoveDirectoryA(R "\\a\\b"); RemoveDirectoryA(R "\\a"); RemoveDirectoryA(R);
}
static void setup(void)
{
    cleanup();
    CreateDirectoryA(R, NULL); CreateDirectoryA(R "\\sub", NULL); CreateDirectoryA(R "\\sub\\in", NULL);
    CreateDirectoryA(R "\\a", NULL); CreateDirectoryA(R "\\a\\b", NULL);
}

TC_CASE(trailing_slashes)
{
    setup();
    v_p = R "/sub/"; CHECK_EQ_I(_chdir(v_p), 0);
    restore();
    v_p = R "\\sub\\"; CHECK_EQ_I(_chdir(v_p), 0);
    getcwdw(now); CHECK(lstrlenA(now) > lstrlenA(saved));
    restore();
}
TC_CASE(dotdot_normalise)
{
    setup();
    v_p = R "\\sub\\..\\sub"; CHECK_EQ_I(_chdir(v_p), 0);
    getcwdw(now);
    CHECK(lstrlenA(now) >= 3 && now[lstrlenA(now) - 3] == 's' && now[lstrlenA(now) - 2] == 'u' && now[lstrlenA(now) - 1] == 'b');
    restore();
}
TC_CASE(dot_unchanged)
{
    setup();
    v_p = "."; CHECK_EQ_I(_chdir(v_p), 0);
    getcwdw(now); CHECK_STR(now, saved);
}
TC_CASE(nested_forward)
{
    setup();
    v_p = R "/a/b"; CHECK_EQ_I(_chdir(v_p), 0);
    getcwdw(now); CHECK(lstrlenA(now) > lstrlenA(saved));
    restore();
    getcwdw(now); CHECK_STR(now, saved);
}
TC_CASE(rmdir_then_chdir_fails)
{
    setup();
    v_p = R "\\sub\\in\\"; CHECK_EQ_I(_rmdir(v_p), 0);
    errno = 0; CHECK_EQ_I(_chdir(v_p), -1); CHECK_EQ_I(errno, ENOENT);
    getcwdw(now); CHECK_STR(now, saved);
    v_p = R "\\mk"; CHECK_EQ_I(_mkdir(v_p), 0);
    CHECK_EQ_I(_rmdir(v_p), 0);
}
int main(void)
{
    tc_begin(); getcwdw(saved);
    TC_RUN(trailing_slashes); TC_RUN(dotdot_normalise); TC_RUN(dot_unchanged); TC_RUN(nested_forward); TC_RUN(rmdir_then_chdir_fails);
    cleanup();
    return tc_end();
}
