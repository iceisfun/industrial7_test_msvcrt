/* TC
 * id:    env_neg_probe_getenv_s_huge_size
 * area:  env
 * kind:  negative
 * funcs: _putenv
 * procs: getenv_s _wgetenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs getenv_s
 * note:  PROBE: sizeInTChars = 0x7fffffff with a small valid buffer. Expected: success, only value+NUL written, no
 *        buffer fill (the debug CRT fills with 0xFE only in debug builds), and no EINVAL from a size sanity check.
 *        getenv_s/_wgetenv_s resolved via procs because the old import libs lack them.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(int, getenv_s, (size_t *, char *, size_t, const char *));
TC_DECL_PROC(int, _wgetenv_s, (size_t *, wchar_t *, size_t, const wchar_t *));
static volatile size_t huge = 0x7fffffff;

TC_CASE(narrow_huge_size_ok)
{
    char buf[16]; size_t r = 99; int i, ok = 1;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    _putenv("TC_HUGE=abc");
    for (i = 0; i < 16; i++) buf[i] = 'Q';
    CHECK_EQ_I(tc_p_getenv_s(&r, buf, huge, "TC_HUGE"), 0);
    CHECK_EQ_U(r, 4);
    CHECK_STR(buf, "abc");
    for (i = 5; i < 16; i++) if (buf[i] != 'Q') ok = 0;
    CHECK(ok);
}
TC_CASE(wide_huge_size_ok)
{
    wchar_t buf[16]; size_t r = 99; int i, ok = 1;
    if (!tc_p__wgetenv_s) TC_SKIP("_wgetenv_s not exported");
    _putenv("TC_HUGE=abc");
    for (i = 0; i < 16; i++) buf[i] = L'Q';
    CHECK_EQ_I(tc_p__wgetenv_s(&r, buf, huge, L"TC_HUGE"), 0);
    CHECK_EQ_U(r, 4);
    CHECK_WSTR(buf, L"abc");
    for (i = 5; i < 16; i++) if (buf[i] != L'Q') ok = 0;
    CHECK(ok);
    _putenv("TC_HUGE=");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(getenv_s); TC_LOAD_PROC(_wgetenv_s);
    TC_RUN(narrow_huge_size_ok); TC_RUN(wide_huge_size_ok);
    return tc_end();
}
