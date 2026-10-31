/* TC
 * id:    gsf_neg_getws_s_null_zero
 * area:  stdio_file
 * funcs: _getws_s freopen
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs gets_s, _getws_s (parameter validation: NULL buffer or zero size -> EINVAL, invalid-parameter handler)
 * note:  Run in a child with stdin redirected to an empty file so no console read can ever happen; handler installed.
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
#define INF "tc_gsf_neg_getws_s_null_zero_in_" SFX ".tmp"

static int child(const char *c)
{
    wchar_t buf[8]; wchar_t *volatile r; wchar_t *volatile nb = NULL; volatile size_t z = 0, n8 = 8; int e;
    if (!freopen(INF, "r", stdin)) return 3;
    tc_iph_install();
    errno = 0;
    if (is(c, "null")) r = _getws_s(nb, n8);
    else if (is(c, "zero")) r = _getws_s(buf, z);
    else if (is(c, "both")) r = _getws_s(nb, z);
    else return 99;
    e = errno;
    if (r != NULL) return 10;
    if (e != EINVAL) return 11;
    if (tc_iph_count != 1) return 12;
    return 0;
}
static void run(const char *name)
{
    DWORD code = 99;
    mk(INF, "", 0);
    CHECK(tc_spawn_self(name, &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(INF);
}
TC_CASE(null_buffer) { run("null"); }
TC_CASE(zero_size) { run("zero"); }
TC_CASE(null_and_zero) { run("both"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(null_buffer); TC_RUN(zero_size); TC_RUN(null_and_zero);
    return tc_end();
}
