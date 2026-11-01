/* TC
 * id:    gsf_neg_getws_s_default_handler_fatal
 * area:  stdio_file
 * funcs: _getws_s freopen
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: with the default invalid-parameter handler the process terminates
 * note:  Child only; exit status is not asserted beyond non-zero and not 77 (77 = call returned). stdin redirected to an empty file.
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
#define INF "tc_gsf_neg_getws_s_default_handler_fatal_in_" SFX ".tmp"

static void chk(const char *name)
{
    DWORD c = 0; int ran;
    mk(INF, "", 0);
    ran = tc_spawn_self(name, &c, 20000);
    CHECK(ran); CHECK(c != 0); CHECK(c != 77);
    DeleteFileA(INF);
}
TC_CASE(null_fatal) { chk("null"); }
TC_CASE(size_zero_fatal) { chk("zero"); }
int main(void)
{
    const char *c;
    tc_begin();
    if (tc_child_name()) tc_iph_default();
    if ((c = tc_child_name()) != NULL) {
        wchar_t buf[8]; wchar_t *volatile r; wchar_t *volatile nb = NULL; volatile size_t z = 0, n8 = 8;
        if (!freopen(INF, "r", stdin)) return 3;
        if (is(c, "null")) r = _getws_s(nb, n8); else r = _getws_s(buf, z);
        (void)r;
        return 77;
    }
    TC_RUN(null_fatal); TC_RUN(size_zero_fatal);
    return tc_end();
}
