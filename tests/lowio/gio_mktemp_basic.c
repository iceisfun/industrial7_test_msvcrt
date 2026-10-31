/* TC
 * id:    gio_mktemp_basic
 * area:  lowio
 * funcs: _mktemp _wmktemp _access _waccess
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mktemp, _wmktemp (template of six X replaced by unique name); MS docs _access
 * note:  Pure name generation, no file is created.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

static int alnum(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

TC_CASE(returns_same_pointer)
{
    char buf[32] = "tcgioXXXXXX"; char *volatile r = _mktemp(buf);
    CHECK_EQ_P(r, buf);
}
TC_CASE(prefix_preserved)
{
    char buf[32] = "tcgioXXXXXX"; char *r = _mktemp(buf);
    CHECK(r != NULL);
    CHECK(memcmp(buf, "tcgio", 5) == 0);
    CHECK_EQ_U(strlen(buf), 11);
}
TC_CASE(x_replaced)
{
    char buf[32] = "tcgioXXXXXX"; int i; char *r = _mktemp(buf);
    CHECK(r != NULL);
    for (i = 5; i < 11; i++) { CHECK(buf[i] != 'X'); CHECK(alnum(buf[i])); }
}
TC_CASE(name_not_existing)
{
    char buf[32] = "tcgioXXXXXX"; char *r = _mktemp(buf);
    CHECK(r != NULL);
    errno = 0;
    CHECK_EQ_I(_access(buf, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wide_variant)
{
    wchar_t buf[32] = L"tcgioXXXXXX"; int i; wchar_t *volatile r = _wmktemp(buf);
    CHECK(r == buf);
    CHECK(wmemcmp(buf, L"tcgio", 5) == 0);
    CHECK_EQ_U(wcslen(buf), 11);
    for (i = 5; i < 11; i++) { CHECK(buf[i] != L'X'); CHECK(buf[i] < 128 && alnum((int)buf[i])); }
    errno = 0;
    CHECK_EQ_I(_waccess(buf, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
}
int main(void)
{
    tc_begin();
    TC_RUN(returns_same_pointer); TC_RUN(prefix_preserved); TC_RUN(x_replaced); TC_RUN(name_not_existing); TC_RUN(wide_variant);
    return tc_end();
}
