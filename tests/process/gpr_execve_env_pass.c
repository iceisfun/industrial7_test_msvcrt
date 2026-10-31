/* TC
 * id:    gpr_execve_env_pass
 * area:  process
 * funcs: _execve
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _exec, _wexec functions (envp array becomes the new environment)
 * note:  Same self-target + temp-file channel as the execle test.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

#define OBS "tc_gpr_execve_obs.tmp"
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

static int child(const char *c)
{
    const char *av[] = { "tc", MARK, NULL };
    const char *e2[] = { "TC_GPR_E1=one", "TC_GPR_E2=two", NULL };
    if (tc_strequ(c, "two")) _execve(g_exe, av, e2);
    return 77;
}
TC_CASE(envp_with_two_vars_both_visible)
{
    DWORD code; char v[64]; int ok = run_case("two", &code);
    CHECK(ok); CHECK(code != 77);
    obs_val("E1", v, sizeof v); CHECK_STR(v, "one");
    obs_val("E2", v, sizeof v); CHECK_STR(v, "two");
}
TC_CASE(parent_only_var_not_inherited)
{
    DWORD code; char v[64]; int ok = run_case("two", &code);
    CHECK(ok); CHECK(code != 77);
    obs_val("INH", v, sizeof v); CHECK_STR(v, "<unset>");
    obs_val("TCN", v, sizeof v); CHECK_STR(v, "2");
}
int main(void)
{
    MAIN_BEGIN()
    TC_RUN(envp_with_two_vars_both_visible); TC_RUN(parent_only_var_not_inherited);
    obs_clean(); return tc_end();
}
