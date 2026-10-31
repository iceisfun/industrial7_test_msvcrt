/* TC
 * id:    gsp_wexeclpe_replaces_process_probe
 * area:  process
 * funcs: _wexeclpe
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _exec, _wexec (environment passed via envp)
 * note:  PROBE: on Windows _wexeclpe creates the successor process and the caller exits with status 0 without waiting.
 *        Case 1 probes that the successor sees envp (GSP_E=7) by writing it to a marker file tc_gsp_wexeclpe_*.tmp
 *        which the parent polls for (<=10s). Case 2 probes that the caller's exit code is 0 and _wexeclpe did not return.
 *        Marker is always removed.
 */
#include "tc.h"
#include <process.h>
#include <wchar.h>

static wchar_t g_self[MAX_PATH], g_marker[MAX_PATH];

static int stage1(void)
{
    wchar_t sr[MAX_PATH + 16] = L"SystemRoot=", ev[] = L"GSP_E=7";
    wchar_t mk[MAX_PATH + 16] = L"GSP_MARK=";
    const wchar_t *envp[4]; int n = 0, i;
    DWORD l;
    l = GetEnvironmentVariableW(L"GSP_MARK", mk + 9, MAX_PATH);
    if (!l || l >= MAX_PATH) return 71;
    for (i = 0; sr[i]; i++) ;
    l = GetEnvironmentVariableW(L"SystemRoot", sr + i, MAX_PATH);
    if (!l || l >= MAX_PATH) sr[i] = 0;
    envp[n++] = ev; envp[n++] = mk; envp[n++] = sr; envp[n] = NULL;
    /* arg0 + marker-mode token; PATH search is irrelevant because a full path is given */
    _wexeclpe(g_self, g_self, L"--tc-child=stage2", (wchar_t *)NULL, envp);
    return 77;   /* returned: failure */
}
static int stage2(void)
{
    wchar_t mk[MAX_PATH], v[16]; HANDLE h; DWORD n;
    if (!GetEnvironmentVariableW(L"GSP_MARK", mk, MAX_PATH)) return 81;
    n = GetEnvironmentVariableW(L"GSP_E", v, 16);
    h = CreateFileW(mk, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 82;
    if (n == 1) WriteFile(h, v, 1, &n, NULL); else WriteFile(h, "?", 1, &n, NULL);
    CloseHandle(h);
    return 0;
}
static int wait_marker(char *out)
{
    int i; DWORD n;
    for (i = 0; i < 100; i++) {
        HANDLE h = CreateFileW(g_marker, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (h != INVALID_HANDLE_VALUE) {
            n = 0; ReadFile(h, out, 1, &n, NULL); CloseHandle(h);
            if (n == 1) return 1;
        }
        Sleep(100);
    }
    return 0;
}
TC_CASE(successor_sees_env)
{
    char c = 0; DWORD code = 99; int ran, got;
    DeleteFileW(g_marker);
    SetEnvironmentVariableW(L"GSP_MARK", g_marker);
    ran = tc_spawn_self("stage1", &code, 20000);
    SetEnvironmentVariableW(L"GSP_MARK", NULL);
    CHECK(ran);
    got = wait_marker(&c);
    CHECK(got);
    CHECK_EQ_I(c, '7');
    DeleteFileW(g_marker);
}
TC_CASE(caller_exit_zero)
{
    DWORD code = 99; int ran; char c;
    DeleteFileW(g_marker);
    SetEnvironmentVariableW(L"GSP_MARK", g_marker);
    ran = tc_spawn_self("stage1", &code, 20000);
    SetEnvironmentVariableW(L"GSP_MARK", NULL);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
    wait_marker(&c);   /* let the successor finish before cleanup */
    DeleteFileW(g_marker);
}
int main(void)
{
    const char *c;
    tc_begin();
    GetModuleFileNameW(NULL, g_self, MAX_PATH);
    if ((c = tc_child_name()) != NULL) {
        if (tc_strequ(c, "stage1")) return stage1();
        if (tc_strequ(c, "stage2")) return stage2();
        return 90;
    }
    { DWORD n = GetCurrentDirectoryW(MAX_PATH - 40, g_marker); lstrcpyW(g_marker + n, L"\\tc_gsp_wexeclpe_1.tmp"); }
    TC_RUN(successor_sees_env); TC_RUN(caller_exit_zero);
    return tc_end();
}
