/* TC
 * id:    env_neg_deleted_not_visible_any_api
 * area:  env
 * kind:  negative
 * funcs: _putenv getenv _wgetenv getenv_s _dupenv_s _wdupenv_s __p__environ
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv ("NAME=" removes the variable), getenv_s (not found: returnValue 0), _dupenv_s (not found: buffer NULL, count 0)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

char ***__cdecl __p__environ(void);
static volatile size_t vsz = 16;

TC_CASE(setup_then_delete)
{
    CHECK_EQ_I(_putenv("TC_DEL=abc"), 0);
    CHECK_STR(getenv("TC_DEL"), "abc");
    CHECK_EQ_I(_putenv("TC_DEL="), 0);
}
TC_CASE(getenv_and_wgetenv_null)
{
    CHECK(getenv("TC_DEL") == NULL);
    CHECK(_wgetenv(L"TC_DEL") == NULL);
}
TC_CASE(getenv_s_not_found)
{
    size_t r = 99; char buf[16]; errno_t e;
    buf[0] = 'z';
    (void)buf;
    e = getenv_s(&r, buf, vsz, "TC_DEL");
    CHECK_EQ_I(e, 0);
    CHECK_EQ_U(r, 0);
}
TC_CASE(dupenv_s_not_found)
{
    char *b = (char *)1; size_t n = 99; errno_t e = _dupenv_s(&b, &n, "TC_DEL");
    CHECK_EQ_I(e, 0); CHECK(b == NULL); CHECK_EQ_U(n, 0);
    if (b != (char *)1) free(b);
}
TC_CASE(wdupenv_s_not_found)
{
    wchar_t *b = (wchar_t *)1; size_t n = 99; errno_t e = _wdupenv_s(&b, &n, L"TC_DEL");
    CHECK_EQ_I(e, 0); CHECK(b == NULL); CHECK_EQ_U(n, 0);
    if (b != (wchar_t *)1) free(b);
}
TC_CASE(environ_has_no_entry)
{
    char **e = *__p__environ(); int i, j, found = 0; const char *p = "TC_DEL=";
    CHECK(e != NULL);
    for (i = 0; e && e[i]; i++) { for (j = 0; p[j] && e[i][j] == p[j]; j++) ; if (!p[j]) found = 1; }
    CHECK(!found);
    if (e) CHECK(e[i] == NULL);
}
TC_CASE(win32_not_found)
{
    char buf[8]; DWORD n;
    SetLastError(0);
    n = GetEnvironmentVariableA("TC_DEL", buf, sizeof buf);
    CHECK_EQ_U(n, 0);
    CHECK_EQ_U(GetLastError(), ERROR_ENVVAR_NOT_FOUND);
}
int main(void)
{
    tc_begin();
    TC_RUN(setup_then_delete); TC_RUN(getenv_and_wgetenv_null); TC_RUN(getenv_s_not_found);
    TC_RUN(dupenv_s_not_found); TC_RUN(wdupenv_s_not_found); TC_RUN(environ_has_no_entry); TC_RUN(win32_not_found);
    return tc_end();
}
