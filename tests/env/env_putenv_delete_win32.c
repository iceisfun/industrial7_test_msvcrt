/* TC
 * id:    env_putenv_delete_win32
 * area:  env
 * funcs: _putenv getenv
 * crt:   all
 * tier:  p1

 * conf:  observed
 * ref:   MS docs _putenv (the CRT updates the process environment); GetEnvironmentVariable
 * note:  Observed: _putenv/_wputenv call SetEnvironmentVariable so the Win32 view tracks the CRT view, including deletion (ERROR_ENVVAR_NOT_FOUND).
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(set_visible_to_win32)
{
    char b[16]; DWORD n;
    _putenv("TC_PW1=winval");
    n = GetEnvironmentVariableA("TC_PW1", b, sizeof b);
    CHECK_EQ_U(n, 6); CHECK_STR(b, "winval");
}
TC_CASE(delete_removes_from_win32_view)
{
    char b[16]; DWORD n;
    _putenv("TC_PW2=x");
    CHECK_EQ_I(_putenv("TC_PW2="), 0);
    SetLastError(0);
    n = GetEnvironmentVariableA("TC_PW2", b, sizeof b);
    CHECK_EQ_U(n, 0); CHECK_EQ_U(GetLastError(), ERROR_ENVVAR_NOT_FOUND);
    CHECK(getenv("TC_PW2") == NULL);
}
int main(void) { tc_begin(); TC_RUN(set_visible_to_win32); TC_RUN(delete_removes_from_win32_view); return tc_end(); }
