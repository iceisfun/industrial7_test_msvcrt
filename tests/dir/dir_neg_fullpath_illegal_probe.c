/* TC
 * id:    dir_neg_fullpath_illegal_probe
 * area:  dir
 * kind:  negative
 * funcs: _fullpath
 * procs: _wfullpath
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _fullpath (implemented over GetFullPathName, which does not validate characters)
 * note:  Probes that _fullpath does not reject syntactically odd input: "a<b" yields non-NULL; "con" yields non-NULL (device
 *        form); "a\\\\b" collapses the doubled separator to "<cwd>\a\b"; a 300-char single component with a 512-char buffer
 *        yields non-NULL (skipped when the cwd is so long the result cannot fit).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_lt = "a<b";
static const char *volatile v_con = "con";
static const char *volatile v_dbl = "a\\\\b";
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(illegal_char)
{
    char buf[MAX_PATH]; char *p = _fullpath(buf, v_lt, MAX_PATH);
    CHECK(p != NULL);
}
TC_CASE(device_name)
{
    char buf[MAX_PATH]; char *p = _fullpath(buf, v_con, MAX_PATH);
    CHECK(p != NULL);
}
TC_CASE(double_separator)
{
    char buf[MAX_PATH], want[MAX_PATH + 8]; char *p; size_t n;
    GetCurrentDirectoryA(MAX_PATH, want); n = slen(want);
    if (n + 5 >= MAX_PATH) TC_SKIP("cwd too long");
    if (n && want[n - 1] != '\\') want[n++] = '\\';
    want[n++] = 'a'; want[n++] = '\\'; want[n++] = 'b'; want[n] = 0;
    p = _fullpath(buf, v_dbl, MAX_PATH);
    CHECK(p != NULL);
    if (p) CHECK_STR(buf, want);
}
TC_CASE(long_component)
{
    char in[310], cwd[MAX_PATH], buf[512]; char *p; int i;
    GetCurrentDirectoryA(MAX_PATH, cwd);
    if (slen(cwd) + 305 >= 512) TC_SKIP("cwd too long");
    for (i = 0; i < 300; i++) in[i] = 'q';
    in[300] = 0;
    p = _fullpath(buf, in, 512);
    CHECK(p != NULL);
}

int main(void)
{
    tc_begin();
    TC_RUN(illegal_char); TC_RUN(device_name); TC_RUN(double_separator); TC_RUN(long_component);
    return tc_end();
}
