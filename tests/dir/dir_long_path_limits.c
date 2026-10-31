/* TC
 * id:    dir_long_path_limits
 * area:  dir
 * funcs: _mkdir _rmdir _chdir _getcwd _fullpath
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mkdir, _chdir, _getcwd, _fullpath; Win32 naming files (MAX_PATH 260, component 255)
 * note:  A directory name must fit in 247 chars of total path for CreateDirectory without long-path opt-in, so the
 *        long component is sized from the current directory length (up to 255). The 256-char component failure is in
 *        dir_neg_mkdir_long. The 260-char _fullpath case asserts only the NULL return (errno is probed elsewhere).
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_name;

static void fill(char *p, char ch, int n) { int i; for (i = 0; i < n; i++) p[i] = ch; p[n] = 0; }

TC_CASE(long_component_roundtrip)
{
    char cwd[MAX_PATH], name[300], now[MAX_PATH], got[MAX_PATH]; DWORD L, a; int n;
    L = GetCurrentDirectoryA(MAX_PATH, cwd);
    if (!L || L >= MAX_PATH) TC_SKIP("cwd");
    if (cwd[L - 1] == '\\') TC_SKIP("cwd is a root");
    n = 240 - (int)L - 1; if (n > 255) n = 255;
    if (n < 16) TC_SKIP("cwd too long for a long component");
    fill(name, 'x', n); name[0] = 't'; name[1] = 'c'; name[2] = '_';
    v_name = name;
    CHECK_EQ_I(_mkdir(v_name), 0);
    a = GetFileAttributesA(name);
    CHECK(a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY));
    CHECK_EQ_I(_chdir(v_name), 0);
    CHECK(_getcwd(got, MAX_PATH) == got);
    GetCurrentDirectoryA(MAX_PATH, now);
    CHECK_STR(got, now);
    CHECK_EQ_I(lstrlenA(got), (int)L + 1 + n);
    CHECK_EQ_I(_chdir(cwd), 0);
    CHECK_EQ_I(_rmdir(v_name), 0);
    CHECK_EQ_U(GetFileAttributesA(name), INVALID_FILE_ATTRIBUTES);
    _chdir(cwd); _rmdir(name);
}

TC_CASE(getcwd_max_path_from_subdir)
{
    char cwd[MAX_PATH], buf[MAX_PATH], now[MAX_PATH]; DWORD L;
    static const char sub[] = "tc_dir_long_sub.tmp";
    L = GetCurrentDirectoryA(MAX_PATH, cwd);
    if (!L || L + 25 >= MAX_PATH) TC_SKIP("cwd");
    v_name = sub;
    CHECK_EQ_I(_mkdir(v_name), 0);
    CHECK_EQ_I(_chdir(v_name), 0);
    CHECK(_getcwd(buf, MAX_PATH) == buf);
    GetCurrentDirectoryA(MAX_PATH, now);
    CHECK_STR(buf, now);
    CHECK_EQ_I(_chdir(cwd), 0);
    CHECK_EQ_I(_rmdir(v_name), 0);
    _chdir(cwd); _rmdir(sub);
}

static int make_path(char *p, int blen)
{
    char cwd[MAX_PATH]; int i = 0;
    if (!GetCurrentDirectoryA(MAX_PATH, cwd) || cwd[1] != ':') return 0;
    if (cwd[0] >= 'a' && cwd[0] <= 'z') cwd[0] = (char)(cwd[0] - 'a' + 'A');   /* keep drive-letter case out of the comparison */
    p[i++] = cwd[0]; p[i++] = ':'; p[i++] = '\\';
    fill(p + i, 'a', 200); i += 200; p[i++] = '\\';
    fill(p + i, 'b', blen);
    return 1;
}

TC_CASE(fullpath_259_fits)
{
    char in[300], out[260];
    if (!make_path(in, 55)) TC_SKIP("cwd has no drive letter");
    CHECK_EQ_I(lstrlenA(in), 259);
    v_name = in;
    CHECK(_fullpath(out, v_name, 260) == out);
    CHECK_STR(out, in);
}

TC_CASE(fullpath_260_too_long)
{
    char in[300], out[MAX_PATH];
    if (!make_path(in, 56)) TC_SKIP("cwd has no drive letter");
    CHECK_EQ_I(lstrlenA(in), 260);
    v_name = in; out[0] = 'Q';
    CHECK(_fullpath(out, v_name, MAX_PATH) == NULL);   /* errno ERANGE is probed in dir_neg_probe_fullpath_erange */
}

int main(void)
{
    tc_begin();
    TC_RUN(long_component_roundtrip); TC_RUN(getcwd_max_path_from_subdir);
    TC_RUN(fullpath_259_fits); TC_RUN(fullpath_260_too_long);
    return tc_end();
}
