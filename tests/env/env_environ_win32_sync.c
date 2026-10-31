/* TC
 * id:    env_environ_win32_sync
 * area:  env
 * funcs: _putenv getenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  observed
 * ref:   _putenv updates the process environment block via SetEnvironmentVariable (inherited by children); kernel32 is the oracle
 */
#include "tc.h"
#include <stdlib.h>

static DWORD w32(const char *n, char *buf, DWORD sz) { return GetEnvironmentVariableA(n, buf, sz); }

TC_CASE(visible)
{
    char b[64]; DWORD n;
    CHECK_EQ_I(_putenv("TC_SYNC_A=winvalue"), 0);
    n = w32("TC_SYNC_A", b, sizeof b);
    CHECK_EQ_I(n, 8);
    CHECK_STR(b, "winvalue");
    CHECK_STR(getenv("TC_SYNC_A"), "winvalue");
    _putenv("TC_SYNC_A=");
}
TC_CASE(replace)
{
    char b[64];
    _putenv("TC_SYNC_B=old");
    CHECK_EQ_I(_putenv("TC_SYNC_B=newer"), 0);
    CHECK_EQ_I(w32("TC_SYNC_B", b, sizeof b), 5);
    CHECK_STR(b, "newer");
    _putenv("TC_SYNC_B=");
}
TC_CASE(delete_)
{
    char b[16];
    _putenv("TC_SYNC_C=1");
    CHECK_EQ_I(w32("TC_SYNC_C", b, sizeof b), 1);
    CHECK_EQ_I(_putenv("TC_SYNC_C="), 0);
    SetLastError(0);
    CHECK_EQ_I(w32("TC_SYNC_C", b, sizeof b), 0);
    CHECK_EQ_I(GetLastError(), ERROR_ENVVAR_NOT_FOUND);
    CHECK(getenv("TC_SYNC_C") == NULL);
}
TC_CASE(win32_case_insensitive)
{
    char b[16];
    _putenv("TC_SYNC_D=cs");
    CHECK_EQ_I(w32("tc_sync_d", b, sizeof b), 2);
    _putenv("TC_SYNC_D=");
}
int main(void)
{
    tc_begin();
    TC_RUN(visible); TC_RUN(replace); TC_RUN(delete_); TC_RUN(win32_case_insensitive);
    return tc_end();
}
