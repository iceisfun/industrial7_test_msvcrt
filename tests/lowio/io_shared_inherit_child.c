/* TC
 * id:    io_shared_inherit_child
 * area:  lowio
 * funcs: _open _close _get_osfhandle _unlink
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _open (_O_NOINHERIT: "Prevents creation of a shared file descriptor"); CreateProcess bInheritHandles
 * note:  A real child process is started with bInheritHandles=TRUE. The child receives the raw HANDLE values on its command
 *        line and checks with GetHandleInformation whether they are valid in its handle table. Default fds must be
 *        inheritable (valid in child); _O_NOINHERIT fds must not be.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

static const char *volatile fn1 = "tc_io_shared_inherit_a.tmp";
static const char *volatile fn2 = "tc_io_shared_inherit_b.tmp";

static void utoa_(ULONG_PTR v, char *o) { char t[24]; int i = 0, j = 0; do { t[i++] = (char)('0' + v % 10); v /= 10; } while (v); while (i) o[j++] = t[--i]; o[j] = 0; }
static ULONG_PTR atou_(const char *s) { ULONG_PTR v = 0; while (*s >= '0' && *s <= '9') v = v * 10 + (*s++ - '0'); return v; }

/* child: command line "... --tc-child=probe H1 H2" ; exit code bit0 = H1 valid, bit1 = H2 valid */
static int child_main(void)
{
    const char *c = GetCommandLineA(); ULONG_PTR h[2]; int n = 0, code = 0; DWORD fl;
    { const char *k = "--tc-child=probe"; int i; /* match the full marker: the exe path itself might contain "probe" or digits */
      for (; *c; c++) { for (i = 0; k[i] && c[i] == k[i]; i++) ; if (!k[i]) { c += i; break; } } }
    while (*c && n < 2) {
        while (*c && (*c < '0' || *c > '9')) c++;
        if (!*c) break;
        h[n++] = atou_(c);
        while (*c >= '0' && *c <= '9') c++;
    }
    if (n != 2) return 99;
    /* a bare numeric handle value may coincide with an unrelated handle in the child: also match type and size */
    if (GetHandleInformation((HANDLE)h[0], &fl) && GetFileType((HANDLE)h[0]) == FILE_TYPE_DISK && GetFileSize((HANDLE)h[0], NULL) == 5) code |= 1;
    if (GetHandleInformation((HANDLE)h[1], &fl) && GetFileType((HANDLE)h[1]) == FILE_TYPE_DISK && GetFileSize((HANDLE)h[1], NULL) == 7) code |= 2;
    return code;
}

TC_CASE(child_sees_handles)
{
    int fa, fb; char exe[MAX_PATH], cmd[MAX_PATH + 100], n1[24], n2[24]; int i = 0, j = 0;
    STARTUPINFOA si; PROCESS_INFORMATION pi; DWORD code = 0xffff; const char *s;
    _unlink(fn1); _unlink(fn2);
    fa = _open(fn1, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    fb = _open(fn2, _O_CREAT | _O_RDWR | _O_BINARY | _O_NOINHERIT, _S_IREAD | _S_IWRITE);
    CHECK(fa >= 0); CHECK(fb >= 0);
    if (fa < 0 || fb < 0) { if (fa >= 0) _close(fa); if (fb >= 0) _close(fb); return; }
    CHECK_EQ_I(_write(fa, "AAAAA", 5), 5); CHECK_EQ_I(_write(fb, "BBBBBBB", 7), 7);
    utoa_((ULONG_PTR)_get_osfhandle(fa), n1); utoa_((ULONG_PTR)_get_osfhandle(fb), n2);
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    cmd[j++] = '"'; for (i = 0; exe[i]; i++) cmd[j++] = exe[i]; cmd[j++] = '"';
    for (s = " --tc-child=probe "; *s; s++) cmd[j++] = *s;
    for (s = n1; *s; s++) cmd[j++] = *s;
    cmd[j++] = ' ';
    for (s = n2; *s; s++) cmd[j++] = *s;
    cmd[j] = 0;
    for (i = 0; i < (int)sizeof si; i++) ((char *)&si)[i] = 0;
    si.cb = sizeof si;
    if (CreateProcessA(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CHECK(WaitForSingleObject(pi.hProcess, 20000) == WAIT_OBJECT_0);
        GetExitCodeProcess(pi.hProcess, &code);
        CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    } else CHECK(!"CreateProcess failed");
    CHECK_EQ_U(code, 1);   /* default handle inherited, NOINHERIT handle not */
    _close(fa); _close(fb); _unlink(fn1); _unlink(fn2);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(child_sees_handles);
    return tc_end();
}
