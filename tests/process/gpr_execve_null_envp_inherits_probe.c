/* TC
 * id:    gpr_execve_null_envp_inherits_probe
 * area:  process
 * funcs: _execve _execvpe _wexecve
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _exec, _wexec functions (envp may be NULL: the new process inherits the caller's environment)
 * note:  PROBES that envp == NULL makes the new image see the intermediate's TC_GPR_INH=1. Own test so a mismatch cannot contaminate the
 *        documented env tests. _execvpe uses a CopyFile'd copy in the current directory; _wexecve uses wide strings, observed via ANSI target.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

#define OBS "tc_gpr_nullenv_obs.tmp"
#define MARK "--tc-child=gpr_target"
static char g_exe[MAX_PATH];
static char g_obs[4096];

static void cat(char *b, const char *s) { lstrcatA(b, s); }
static void put_env(char *b, const char *key, const char *name)
{
    char v[256]; DWORD n;
    cat(b, key); cat(b, "=");
    SetLastError(0); n = GetEnvironmentVariableA(name, v, sizeof v);
    if (n == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND) cat(b, "<unset>");
    else if (n < sizeof v) { v[n] = 0; cat(b, v); }
    cat(b, "\n");
}
static void put_tcn(char *b)
{
    char *e = GetEnvironmentStringsA(), *p = e; int n = 0; char t[24];
    for (; p && *p; p += lstrlenA(p) + 1) if (p[0] == 'T' && p[1] == 'C' && p[2] == '_') n++;
    if (e) FreeEnvironmentStringsA(e);
    tc_i64(n, t); cat(b, "TCN="); cat(b, t); cat(b, "\n");
}
/* runs in the exec'd image: records what it sees, atomically publishes OBS */
static void target_observe(void)
{
    static char b[4096]; HANDLE h; DWORD w;
    b[0] = 0;
    cat(b, "CMD="); cat(b, GetCommandLineA()); cat(b, "\n");
    put_env(b, "E1", "TC_GPR_E1"); put_env(b, "E2", "TC_GPR_E2"); put_env(b, "INH", "TC_GPR_INH"); put_tcn(b);
    h = CreateFileA(OBS ".part", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, b, lstrlenA(b), &w, NULL); CloseHandle(h);
    MoveFileExA(OBS ".part", OBS, MOVEFILE_REPLACE_EXISTING);
}
static void obs_clean(void) { DeleteFileA(OBS); DeleteFileA(OBS ".part"); }
static void obs_wait(void)
{
    int i;
    for (i = 0; i < 400; i++) {
        HANDLE h = CreateFileA(OBS, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
        if (h != INVALID_HANDLE_VALUE) { DWORD n = 0; ReadFile(h, g_obs, sizeof g_obs - 1, &n, NULL); g_obs[n] = 0; CloseHandle(h); return; }
        Sleep(50);
    }
}
static void obs_val(const char *key, char *out, int cap)
{
    const char *p = g_obs; int kl = lstrlenA(key), i;
    out[0] = 0;
    while (*p) {
        for (i = 0; i < kl && p[i] == key[i]; i++) ;
        if (i == kl && p[kl] == '=') {
            p += kl + 1; for (i = 0; p[i] && p[i] != '\n' && i < cap - 1; i++) out[i] = p[i];
            out[i] = 0; return;
        }
        while (*p && *p != '\n') p++;
        if (*p) p++;
    }
}
/* command-line text following the marker, trailing blanks trimmed */
static void obs_tail(char *out, int cap)
{
    char cmd[1024]; const char *m = MARK; int i, j, n; out[0] = 0;
    obs_val("CMD", cmd, sizeof cmd);
    for (i = 0; cmd[i]; i++) { for (j = 0; m[j] && cmd[i + j] == m[j]; j++) ; if (!m[j]) break; }
    if (!cmd[i]) return;
    for (n = 0, i += lstrlenA(m); cmd[i] && n < cap - 1; i++) out[n++] = cmd[i];
    while (n && out[n - 1] == ' ') n--;
    out[n] = 0;
}
static void del_retry(const char *f) { int i; for (i = 0; i < 40 && !DeleteFileA(f) && GetLastError() != ERROR_FILE_NOT_FOUND; i++) Sleep(50); }
static int run_case(const char *name, DWORD *code)
{
    int ok; obs_clean(); g_obs[0] = 0; *code = 99;
    SetEnvironmentVariableA("TC_GPR_INH", "1");
    ok = tc_spawn_self(name, code, 20000);
    if (ok && *code != 77) obs_wait();
    return ok;
}
#define MAIN_BEGIN() const char *c; tc_begin(); GetModuleFileNameA(NULL, g_exe, MAX_PATH); \
    if ((c = tc_child_name()) != NULL) { if (tc_strequ(c, "gpr_target")) { target_observe(); return 0; } return child(c); }

#define CP "tc_gpr_nullenv_cp.exe"
static int child(const char *c)
{
    const char *av[] = { "tc", MARK, NULL };
    const wchar_t *wav[] = { L"tc", L"--tc-child=gpr_target", NULL };
    wchar_t wexe[MAX_PATH]; GetModuleFileNameW(NULL, wexe, MAX_PATH);
    if (tc_strequ(c, "ve")) _execve(g_exe, av, NULL);
    if (tc_strequ(c, "vpe")) _execvpe(CP, av, NULL);
    if (tc_strequ(c, "wve")) _wexecve(wexe, wav, NULL);
    return 77;
}
TC_CASE(null_envp_inherits_marker)
{
    const char *n[3]; int i; n[0] = "ve"; n[1] = "vpe"; n[2] = "wve";
    for (i = 0; i < 3; i++) {
        DWORD code; char v[64]; int ok = run_case(n[i], &code);
        CHECK(ok); CHECK(code != 77);
        obs_val("INH", v, sizeof v); CHECK_STR(v, "1");
    }
}
int main(void)
{
    MAIN_BEGIN()
    if (!CopyFileA(g_exe, CP, FALSE)) { tc_write("TC FAIL gpr_execve_null_envp_inherits_probe::copy\n"); return 1; }
    TC_RUN(null_envp_inherits_marker);
    obs_clean(); del_retry(CP); return tc_end();
}
