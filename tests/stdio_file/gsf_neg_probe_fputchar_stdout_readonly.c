/* TC
 * id:    gsf_neg_probe_fputchar_stdout_readonly
 * area:  stdio_file
 * funcs: _fputchar freopen ferror
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.7.8 putchar (write error -> EOF, error indicator); errno value is MS CRT behaviour
 * note:  PROBE: writing to a stream opened read-only returns EOF, sets the error indicator and errno == EBADF
 *        (believed, as for fputc on a read-only stream). The exact errno is the guess.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)
static void mk(const char *n, const void *d, DWORD l)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, d, l, &w, NULL); CloseHandle(h);
}
static int rd(const char *n, unsigned char *b, int cap)  /* bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
#define F "tc_gsf_neg_probe_fputchar_stdout_readonly_" SFX ".tmp"
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }

static int child(const char *c)
{
    if (is(c, "ro")) {
        int r, e;
        if (!freopen(F, "r", stdout)) return 3;
        errno = 0;
        r = _fputchar('x'); e = errno;
        if (r != EOF) return 10;
        if (!ferror(stdout)) return 11;
        if (e != EBADF) return 12;
        return 0;
    }
    return 99;
}
TC_CASE(rdonly_stdout)
{
    DWORD code = 99;
    mk(F, "orig", 4);
    CHECK(tc_spawn_self("ro", &code, 20000)); CHECK_EQ_U(code, 0);
}
TC_CASE(file_unchanged)
{
    unsigned char b[16]; int n = rd(F, b, sizeof b);
    CHECK_EQ_I(n, 4);
    if (n == 4) CHECK_MEM(b, "orig", 4);
    DeleteFileA(F);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(rdonly_stdout); TC_RUN(file_unchanged);
    return tc_end();
}
