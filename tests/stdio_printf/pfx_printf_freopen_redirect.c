/* TC
 * id:    pfx_printf_freopen_redirect
 * area:  stdio_printf
 * funcs: printf freopen fflush
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs freopen, printf, fflush
 * note:  Uses tc_spawn_self; the children redirect their own stdout to a file. The parent never touches its own stdout.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

static const char *path_for(const char *name)
{
    static char p[96]; const char *pre = "tc_pfx_printf_freopen_redirect_"; int i = 0, j = 0;
    while (pre[i]) { p[j++] = pre[i++]; }
    for (i = 0; name[i]; i++) p[j++] = name[i];
    { const char *v = TCV; for (i = 0; v[i]; i++) p[j++] = v[i]; }
    p[j++] = '.'; p[j++] = 't'; p[j++] = 'm'; p[j++] = 'p'; p[j] = 0; return p;
}
static int child_main(const char *name)
{
    const char *mode = (name[0] == 't') ? "w" : "wb";
    FILE *r; int n;
    r = freopen(path_for(name), mode, stdout);
    if (r == NULL) return 10;
    n = printf("x\ny\n");
    if (n != 4) return 11;
    if (fflush(stdout) != 0) return 12;
    n = printf("%d", 305);
    if (n != 3) return 13;
    return 0;                      /* exit() flushes the rest */
}
static void run(const char *name, long size, const void *want)
{
    DWORD code = 99; int ran = tc_spawn_self(name, &code, 20000); long sz;
    CHECK(ran);
    CHECK_EQ_U(code, 0);
    sz = tc_slurp(path_for(name));
    CHECK_EQ_I(sz, size);
    if (sz == size) CHECK_MEM(tc_buf, want, size);
    DeleteFileA(path_for(name));
}
TC_CASE(text_child) { run("text", 9, "x\r\ny\r\n305"); }
TC_CASE(binary_child) { run("bin", 7, "x\ny\n305"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(text_child); TC_RUN(binary_child);
    return tc_end();
}
