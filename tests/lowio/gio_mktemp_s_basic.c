/* TC
 * id:    gio_mktemp_s_basic
 * area:  lowio
 * funcs: _access _waccess
 * procs: _mktemp_s _wmktemp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mktemp_s, _wmktemp_s (returns 0 on success)
 * note:  procs because libmsvcr80.a lacks the _s functions.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

TC_DECL_PROC(int, _mktemp_s, (char *, size_t));
TC_DECL_PROC(int, _wmktemp_s, (wchar_t *, size_t));

static int alnum(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

TC_CASE(returns_zero_and_replaces)
{
    char buf[32] = "tcgioXXXXXX"; int i;
    if (!tc_p__mktemp_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__mktemp_s(buf, sizeof buf), 0);
    CHECK_EQ_U(strlen(buf), 11);
    for (i = 5; i < 11; i++) { CHECK(buf[i] != 'X'); CHECK(alnum(buf[i])); }
}
TC_CASE(size_exact_len_plus_1)
{
    char buf[32] = "tcgioXXXXXX";
    if (!tc_p__mktemp_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__mktemp_s(buf, 12), 0);
    CHECK_EQ_U(strlen(buf), 11);
}
TC_CASE(size_larger_than_needed)
{
    char buf[64] = "tcgioXXXXXX";
    if (!tc_p__mktemp_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__mktemp_s(buf, 64), 0);
    CHECK_EQ_U(strlen(buf), 11);
}
TC_CASE(prefix_preserved)
{
    char buf[32] = "tcgioXXXXXX";
    if (!tc_p__mktemp_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__mktemp_s(buf, sizeof buf), 0);
    CHECK(memcmp(buf, "tcgio", 5) == 0);
}
TC_CASE(name_not_existing)
{
    char buf[32] = "tcgioXXXXXX";
    if (!tc_p__mktemp_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__mktemp_s(buf, sizeof buf), 0);
    errno = 0;
    CHECK_EQ_I(_access(buf, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wide_variant)
{
    wchar_t buf[32] = L"tcgioXXXXXX"; int i;
    if (!tc_p__wmktemp_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__wmktemp_s(buf, 12), 0);
    CHECK_EQ_U(wcslen(buf), 11);
    CHECK(wmemcmp(buf, L"tcgio", 5) == 0);
    for (i = 5; i < 11; i++) { CHECK(buf[i] != L'X'); CHECK(buf[i] < 128 && alnum((int)buf[i])); }
    errno = 0;
    CHECK_EQ_I(_waccess(buf, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_mktemp_s); TC_LOAD_PROC(_wmktemp_s);
    CHECK(tc_p__mktemp_s && tc_p__wmktemp_s);
    TC_RUN(returns_zero_and_replaces); TC_RUN(size_exact_len_plus_1); TC_RUN(size_larger_than_needed);
    TC_RUN(prefix_preserved); TC_RUN(name_not_existing); TC_RUN(wide_variant);
    return tc_end();
}
