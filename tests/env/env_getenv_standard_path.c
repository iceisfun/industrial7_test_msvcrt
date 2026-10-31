/* TC
 * id:    env_getenv_standard_path
 * area:  env
 * funcs: getenv
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs getenv; Win32 GetEnvironmentVariable
 * note:  The CRT copies the process environment at startup; with no modification since, getenv must agree with
 *        GetEnvironmentVariableA. Each comparison is made only if Win32 reports the variable present.
 */
#include "tc.h"
#include <stdlib.h>

static char buf[32768];
static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }

TC_CASE(PATH_matches_GetEnvironmentVariable)
{
    DWORD n = GetEnvironmentVariableA("PATH", buf, sizeof buf);
    char *v = getenv("PATH");
    if (!n || n >= sizeof buf) TC_SKIP("no PATH from Win32");
    CHECK(v != NULL);
    if (v) CHECK_STR(v, buf);
}
TC_CASE(SystemRoot_or_windir_nonnull_when_Win32_has_it)
{
    DWORD n = GetEnvironmentVariableA("SystemRoot", buf, sizeof buf);
    DWORD m = GetEnvironmentVariableA("windir", buf, sizeof buf);
    if (!n && !m) TC_SKIP("neither SystemRoot nor windir");
    CHECK((n && getenv("SystemRoot") != NULL) || (m && getenv("windir") != NULL));
}
TC_CASE(length_equal_to_Win32_length)
{
    DWORD n = GetEnvironmentVariableA("PATH", buf, sizeof buf);
    char *v = getenv("PATH");
    if (!n || n >= sizeof buf) TC_SKIP("no PATH from Win32");
    CHECK(v != NULL);
    if (v) CHECK_EQ_I(slen(v), n);
}

int main(void)
{
    tc_begin();
    TC_RUN(PATH_matches_GetEnvironmentVariable); TC_RUN(SystemRoot_or_windir_nonnull_when_Win32_has_it); TC_RUN(length_equal_to_Win32_length);
    return tc_end();
}
