/* TC
 * id:    dir_searchenv_basic
 * area:  dir
 * funcs: _searchenv _wsearchenv
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _searchenv, _wsearchenv (search current directory first, then each directory of the variable)
 *        the full-path form of the result for a file in the cwd is observed behaviour (_fullpath of the name), not documented
 * note:  Uses a scratch file in the current directory and a variable name that does not exist, so no environment
 *        modification is needed (the CRT keeps its own environment copy).
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

#define FN  "tc_dir_searchenv_basic.tmp"
#define WFN L"tc_dir_searchenv_basic.tmp"

static void mkfile(void)
{
    HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}
static void expected(char *out)
{
    DWORD n = GetCurrentDirectoryA(MAX_PATH, out);
    if (n && out[n - 1] != '\\') out[n++] = '\\';
    lstrcpyA(out + n, FN);
}
static const char *volatile v_fn = FN, *volatile v_var = "TC_DIR_NO_SUCH_VAR";
static const wchar_t *volatile v_wfn = WFN, *volatile v_wvar = L"TC_DIR_NO_SUCH_VAR";

TC_CASE(cwd_first)
{
    char buf[_MAX_PATH], exp[MAX_PATH + 40]; memset(buf, 'x', sizeof buf); buf[sizeof buf - 1] = 0;
    mkfile(); expected(exp);
    _searchenv(v_fn, v_var, buf);
    CHECK(lstrcmpiA(buf, exp) == 0);
    if (lstrcmpiA(buf, exp)) tc_write(buf);
}
TC_CASE(absolute_name)
{
    char buf[_MAX_PATH], exp[MAX_PATH + 40]; const char *volatile in;
    mkfile(); expected(exp); in = exp;
    _searchenv(in, v_var, buf);
    CHECK(lstrcmpiA(buf, exp) == 0);
}
TC_CASE(wide)
{
    wchar_t wbuf[_MAX_PATH], wexp[MAX_PATH + 40]; DWORD n;
    mkfile();
    n = GetCurrentDirectoryW(MAX_PATH, wexp);
    if (!n || n >= MAX_PATH) TC_SKIP("cwd too long");
    if (wexp[n - 1] != L'\\') wexp[n++] = L'\\';
    lstrcpyW(wexp + n, WFN);
    _wsearchenv(v_wfn, v_wvar, wbuf);
    CHECK(lstrcmpiW(wbuf, wexp) == 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(cwd_first); TC_RUN(absolute_name); TC_RUN(wide);
    DeleteFileA(FN);
    return tc_end();
}
