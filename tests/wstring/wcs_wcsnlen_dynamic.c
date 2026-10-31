/* TC
 * id:    wcs_wcsnlen_dynamic
 * area:  wstring
 * funcs:
 * procs: wcsnlen
 * crt:   >=100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strnlen, strnlen_s (wcsnlen: VS2005+ headers; exported by msvcr100+)
 * note:  ucrtbase export absent from mingw lib so resolved at run time
 */
#include "tc.h"
#include <wchar.h>

typedef size_t (__cdecl *wcsnlen_t)(const wchar_t *, size_t);
static wcsnlen_t p_wcsnlen;
static const wchar_t s6[] = { 'a', 'b', 'c', 'd', 'e', 'f', 0 };

TC_CASE(exported) { CHECK(p_wcsnlen != NULL); }
TC_CASE(max_less) { if (!p_wcsnlen) TC_SKIP("no wcsnlen"); CHECK_EQ_U(p_wcsnlen(s6, 3), 3); CHECK_EQ_U(p_wcsnlen(s6, 5), 5); }
TC_CASE(max_equal_len) { if (!p_wcsnlen) TC_SKIP("no wcsnlen"); CHECK_EQ_U(p_wcsnlen(s6, 6), 6); }
TC_CASE(max_greater) { if (!p_wcsnlen) TC_SKIP("no wcsnlen"); CHECK_EQ_U(p_wcsnlen(s6, 100), 6); CHECK_EQ_U(p_wcsnlen(s6, 7), 6); }
TC_CASE(max_zero) { if (!p_wcsnlen) TC_SKIP("no wcsnlen"); CHECK_EQ_U(p_wcsnlen(s6, 0), 0); }
TC_CASE(no_nul_within_max)
{
    /* buffer without terminator: must not read past max */
    static const wchar_t b[4] = { 'w', 'x', 'y', 'z' };
    if (!p_wcsnlen) TC_SKIP("no wcsnlen");
    CHECK_EQ_U(p_wcsnlen(b, 4), 4);
    CHECK_EQ_U(p_wcsnlen(b, 2), 2);
}
int main(void)
{
    tc_begin();
    p_wcsnlen = (wcsnlen_t)tc_crt_proc("wcsnlen");
    TC_RUN(exported); TC_RUN(max_less); TC_RUN(max_equal_len); TC_RUN(max_greater); TC_RUN(max_zero); TC_RUN(no_nul_within_max);
    return tc_end();
}
