/* TC
 * id:    gpr_wexec_wide_args
 * area:  process
 * funcs: _wexecle _wexeclp _wexecve _wexecvp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _exec, _wexec functions (wide-character variants)
 * note:  The exec'd image reads GetCommandLineW / GetEnvironmentVariableW and writes UTF-16 to tc_gpr_wexec_obs.tmp. ASCII-only values (the
 *        non-ASCII probe is gpr_wexec_unicode_args_probe). _wexeclp/_wexecvp use a CopyFile'd copy in the current directory (observed:
 *        p-variants try the bare name in the current directory before PATH).
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

#define OBS "tc_gpr_wexec_obs.tmp"
#define WMARK L"--tc-child=gpr_wtarget"
#define UNI L"wide"
static char g_exe[MAX_PATH];
static wchar_t g_wexe[MAX_PATH];
static wchar_t g_wobs[2048];
static const wchar_t *wline(int k)  /* NUL-terminated copy of line k of the observation */
{
    static wchar_t out[1024]; const wchar_t *p = g_wobs; int n = 0;
    while (k-- > 0) { while (*p && *p != L'\n') p++; if (*p) p++; }
    while (p[n] && p[n] != L'\n' && n < 1023) { out[n] = p[n]; n++; }
    out[n] = 0; return out;
}
static const wchar_t *wtail(void)  /* command line text after the marker */
{
    static wchar_t out[1024]; const wchar_t *l = wline(0), *m = WMARK; int i, j, n = 0; out[0] = 0;
    for (i = 0; l[i]; i++) { for (j = 0; m[j] && l[i + j] == m[j]; j++) ; if (!m[j]) break; }
    if (!l[i]) return out;
    for (i += 22; l[i] && n < 1023; i++) out[n++] = l[i];
    while (n && out[n - 1] == L' ') n--;
    out[n] = 0; return out;
}
static void wtarget(void)
{
    static wchar_t b[2048]; wchar_t v[128]; DWORD n, w; HANDLE h;
    lstrcpyW(b, GetCommandLineW()); lstrcatW(b, L"\n");
    SetLastError(0); n = GetEnvironmentVariableW(L"TC_GPR_W", v, 128);
    if (n == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND) lstrcatW(b, L"<unset>");
    else if (n < 128) { v[n] = 0; lstrcatW(b, v); }
    lstrcatW(b, L"\n");
    h = CreateFileA(OBS ".part", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, b, lstrlenW(b) * sizeof(wchar_t), &w, NULL); CloseHandle(h);
    MoveFileExA(OBS ".part", OBS, MOVEFILE_REPLACE_EXISTING);
}
static void obs_clean(void) { DeleteFileA(OBS); DeleteFileA(OBS ".part"); }
static void del_retry(const char *f) { int i; for (i = 0; i < 40 && !DeleteFileA(f) && GetLastError() != ERROR_FILE_NOT_FOUND; i++) Sleep(50); }
static void obs_wait(void)
{
    int i;
    for (i = 0; i < 400; i++) {
        HANDLE h = CreateFileA(OBS, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
        if (h != INVALID_HANDLE_VALUE) { DWORD n = 0; ReadFile(h, g_wobs, sizeof g_wobs - 2, &n, NULL); g_wobs[n / 2] = 0; CloseHandle(h); return; }
        Sleep(50);
    }
}
static int run_case(const char *name, DWORD *code)
{
    int ok; obs_clean(); g_wobs[0] = 0; *code = 99;
    ok = tc_spawn_self(name, code, 20000);
    if (ok && *code != 77) obs_wait();
    return ok;
}
#define CP "tc_gpr_wexec_cp.exe"
#define WCP L"tc_gpr_wexec_cp.exe"

static int child(const char *c)
{
    const wchar_t *e1[] = { L"TC_GPR_W=" UNI, NULL };
    const wchar_t *e2[] = { L"TC_GPR_W=env2", NULL };
    const wchar_t *av[] = { L"tc", WMARK, L"x" UNI, L"y", NULL };
    if (tc_strequ(c, "le")) _wexecle(g_wexe, L"tc", WMARK, (wchar_t *)NULL, e1);
    if (tc_strequ(c, "lp")) _wexeclp(WCP, L"tc", WMARK, UNI, (wchar_t *)NULL);
    if (tc_strequ(c, "ve")) _wexecve(g_wexe, av, e2);
    if (tc_strequ(c, "vp")) _wexecvp(WCP, av);
    return 77;
}
TC_CASE(wexecle_unicode_env_value)
{
    DWORD code; int ok = run_case("le", &code);
    CHECK(ok); CHECK(code != 77);
    CHECK_WSTR(wline(1), UNI);
}
TC_CASE(wexeclp_bare_name_unicode_arg)
{
    DWORD code; int ok = run_case("lp", &code);
    CHECK(ok); CHECK(code != 77);
    CHECK_WSTR(wtail(), L" " UNI);
}
TC_CASE(wexecve_envp_array)
{
    DWORD code; int ok = run_case("ve", &code);
    CHECK(ok); CHECK(code != 77);
    CHECK_WSTR(wline(1), L"env2");
    CHECK_WSTR(wtail(), L" x" UNI L" y");
}
TC_CASE(wexecvp_argv_array_unicode)
{
    DWORD code; int ok = run_case("vp", &code);
    CHECK(ok); CHECK(code != 77);
    CHECK_WSTR(wtail(), L" x" UNI L" y");
}
int main(void)
{
    const char *c; tc_begin(); GetModuleFileNameA(NULL, g_exe, MAX_PATH); GetModuleFileNameW(NULL, g_wexe, MAX_PATH);
    if ((c = tc_child_name()) != NULL) { if (tc_strequ(c, "gpr_wtarget")) { wtarget(); return 0; } return child(c); }
    if (!CopyFileA(g_exe, CP, FALSE)) { tc_write("TC FAIL gpr_wexec_wide_args::copy\n"); return 1; }
    TC_RUN(wexecle_unicode_env_value); TC_RUN(wexeclp_bare_name_unicode_arg);
    TC_RUN(wexecve_envp_array); TC_RUN(wexecvp_argv_array_unicode);
    obs_clean(); del_retry(CP); return tc_end();
}
