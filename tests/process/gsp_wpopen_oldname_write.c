/* TC
 * id:    gsp_wpopen_oldname_write
 * area:  process
 * funcs: _wpopen _pclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _popen, _wpopen: write/read pipe to a command; _pclose returns its exit status
 * note:  Unprefixed wpopen/pclose (thunks to _wpopen/_pclose). The command is this exe as child: "count" counts stdin bytes
 *        (exit code = total, <=255), "zout" writes the single byte Z to stdout.
 */
#include "tc.h"
#include <stdio.h>

static int child_main(const char *m)
{
    char b[64]; DWORD n, total = 0; HANDLE in = GetStdHandle(STD_INPUT_HANDLE), out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (tc_strequ(m, "count")) {
        while (ReadFile(in, b, sizeof b, &n, NULL) && n) total += n;
        return (int)total;
    }
    if (tc_strequ(m, "zout")) { WriteFile(out, "Z", 1, &n, NULL); return 0; }
    return 90;
}
static wchar_t g_cmd[2 * MAX_PATH + 64];
static void mkcmd(const wchar_t *arg)
{
    wchar_t self[MAX_PATH]; int i = 0, j; const wchar_t *p;
    GetModuleFileNameW(NULL, self, MAX_PATH);
    g_cmd[i++] = L'"'; g_cmd[i++] = L'"';
    for (j = 0; self[j]; j++) g_cmd[i++] = self[j];
    g_cmd[i++] = L'"'; g_cmd[i++] = L' ';
    for (p = arg; *p; p++) g_cmd[i++] = *p;
    g_cmd[i++] = L'"'; g_cmd[i] = 0;
}
TC_CASE(write_5_bytes)
{
    FILE *volatile f; size_t w;
    mkcmd(L"--tc-child=count");
    f = wpopen(g_cmd, L"w");
    CHECK(f != NULL);
    if (!f) return;
    w = fwrite("abcde", 1, 5, f);
    CHECK_EQ_U(w, 5);
    CHECK_EQ_I(pclose(f), 5);
}
TC_CASE(write_0_bytes)
{
    FILE *volatile f;
    mkcmd(L"--tc-child=count");
    f = wpopen(g_cmd, L"w");
    CHECK(f != NULL);
    if (!f) return;
    CHECK_EQ_I(pclose(f), 0);
}
TC_CASE(read_mode)
{
    FILE *volatile f; int ch;
    mkcmd(L"--tc-child=zout");
    f = wpopen(g_cmd, L"r");
    CHECK(f != NULL);
    if (!f) return;
    ch = fgetc(f);
    CHECK_EQ_I(ch, 'Z');
    CHECK_EQ_I(pclose(f), 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(write_5_bytes); TC_RUN(write_0_bytes); TC_RUN(read_mode);
    return tc_end();
}
