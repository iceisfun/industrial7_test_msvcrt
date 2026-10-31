/* TC
 * id:    gsf_neg_probe_getws_s_eof_buffer
 * area:  stdio_file
 * funcs: _getws_s freopen
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs gets_s, _getws_s
 * note:  PROBE: at immediate EOF _getws_s returns NULL (documented); whether it also sets buffer[0] = 0 is believed
 *        (gets_s zeroes the buffer at the start/at EOF) but not documented, and errno is believed unchanged.
 *        stdin is an empty file in a child process.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)
static void mk(const char *n, const void *d, DWORD l)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    if (l) WriteFile(h, d, l, &w, NULL);
    CloseHandle(h);
}
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
static int isw(const wchar_t *a, const wchar_t *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
#define INF "tc_gsf_neg_probe_getws_s_eof_buffer_in_" SFX ".tmp"

static int child(const char *c)
{
    wchar_t buf[8]; wchar_t *volatile r; int e;
    (void)c;
    if (!freopen(INF, "r", stdin)) return 3;
    tc_iph_install();
    buf[0] = L'Q'; buf[1] = 0;
    errno = 0;
    r = _getws_s(buf, 8); e = errno;
    if (r != NULL) return 10;
    if (buf[0] != 0) return 11;
    if (e != 0) return 12;
    if (tc_iph_count != 0) return 13;
    return 0;
}
TC_CASE(eof_buffer_nul)
{
    DWORD code = 99;
    mk(INF, "", 0);
    CHECK(tc_spawn_self("eof", &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(INF);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(eof_buffer_nul);
    return tc_end();
}
