/* TC
 * id:    gsf_neg_probe_gets_null_and_eof_buffer
 * area:  stdio_file
 * funcs:
 * procs: gets
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs gets (deprecated); parameter validation convention of VS2005+ CRTs
 * note:  PROBE: gets(NULL) returns NULL with errno == EINVAL and calls the invalid-parameter handler (believed, not
 *        documented for gets); at EOF gets returns NULL and leaves the caller's buffer untouched (believed).
 *        stdin is redirected to an empty file in the child so no console read can happen.
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
#define IN "tc_gsf_neg_probe_gets_null_and_eof_buffer_in_" SFX ".tmp"
typedef char *(__cdecl *gets_t)(char *);
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }

static int child(const char *c)
{
    gets_t g = (gets_t)tc_crt_proc("gets"); char buf[16]; char *r; int e;
    if (!g) return 2;
    if (!freopen(IN, "r", stdin)) return 3;
    if (!tc_iph_install()) return 4;
    if (is(c, "null")) {
        errno = 0; r = g(NULL); e = errno;
        if (r != NULL) return 10;
        if (e != EINVAL) return 11;
        if (tc_iph_count < 1) return 12;
        return 0;
    }
    if (is(c, "eofbuf")) {
        buf[0] = 'Q'; buf[1] = 'R'; buf[2] = 0;
        r = g(buf);
        if (r != NULL) return 10;
        if (buf[0] != 'Q' || buf[1] != 'R' || buf[2] != 0) return 11;
        return 0;
    }
    return 99;
}
static void run(const char *name)
{
    DWORD code = 99;
    mk(IN, "", 0);
    CHECK(tc_spawn_self(name, &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(IN);
}
TC_CASE(null_buffer) { run("null"); }
TC_CASE(eof_buffer_untouched) { run("eofbuf"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    if (!tc_crt_proc("gets")) { tc_write("TC FAIL " TC_TEST_ID "::__gets not exported\n"); return 1; }
    TC_RUN(null_buffer); TC_RUN(eof_buffer_untouched);
    return tc_end();
}
