/* TC
 * id:    env_neg_probe_wgetenv_odd_names
 * area:  env
 * kind:  negative
 * funcs: _wgetenv _wputenv
 * procs: _wgetenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _wgetenv (NULL if not found); empty and '='-containing names are not documented
 * note:  PROBE: (1) _wgetenv(L"") returns NULL. (2) With TCW_EQ=xyz set, _wgetenv(L"TCW_EQ=xyz") and _wgetenv(L"TCW_EQ=")
 *        return NULL (names containing '=' never match, because the entry's char after the name must be '=').
 *        (3) _wgetenv_s with an empty name returns 0 with *returnValue 0.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

TC_DECL_PROC(int, _wgetenv_s, (size_t *, wchar_t *, size_t, const wchar_t *));
static volatile size_t vsz = 8;

TC_CASE(empty_name_null) { CHECK(_wgetenv(L"") == NULL); }
TC_CASE(name_with_equals_not_found)
{
    CHECK_EQ_I(_wputenv(L"TCW_EQ=xyz"), 0);
    CHECK(_wgetenv(L"TCW_EQ=xyz") == NULL);
    CHECK(_wgetenv(L"TCW_EQ=") == NULL);
    _wputenv(L"TCW_EQ=");
}
TC_CASE(wgetenv_s_empty_name)
{
    size_t r = 99; wchar_t buf[8];
    if (!tc_p__wgetenv_s) TC_SKIP("_wgetenv_s not exported");
    buf[0] = L'z';
    CHECK_EQ_I(tc_p__wgetenv_s(&r, buf, vsz, L""), 0);
    CHECK_EQ_U(r, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wgetenv_s);
    TC_RUN(empty_name_null); TC_RUN(name_with_equals_not_found); TC_RUN(wgetenv_s_empty_name);
    return tc_end();
}
