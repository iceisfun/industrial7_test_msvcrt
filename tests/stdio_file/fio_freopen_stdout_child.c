/* TC
 * id:    fio_freopen_stdout_child
 * area:  stdio_file
 * funcs: freopen fwrite fclose
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.5.4 (freopen commonly used to redirect stdout)
 * note:  Runs in a child (tc_spawn_self) since redirecting stdout of the harness process is undesirable.
 */
#include "tc.h"
#include <stdio.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)

static void mk(const char *n, const char *d)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0, l = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    while (d[l]) l++;
    WriteFile(h, d, l, &w, NULL); CloseHandle(h);
}
static int rd(const char *n, char *b, int cap)  /* returns bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap - 1, &r, NULL)) r = 0;
    CloseHandle(h); b[r] = 0; return (int)r;
}

#define F "tc_fio_freopen_stdout_child_" SFX ".tmp"

TC_CASE(redirect)
{
    DWORD code = 99; char out[16]; int ran;
    DeleteFileA(F);
    ran = tc_spawn_self("redir", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
    CHECK_EQ_I(rd(F, out, sizeof out), 2);
    CHECK_STR(out, "hi");
    DeleteFileA(F);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        FILE *r = freopen(F, "w", stdout);
        if (!r) return 3;
        if (fwrite("hi", 1, 2, stdout) != 2) return 4;
        if (fclose(stdout) != 0) return 5;
        return 0;
    }
    TC_RUN(redirect);
    return tc_end();
}
