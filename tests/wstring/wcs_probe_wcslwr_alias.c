/* TC
 * id:    wcs_probe_wcslwr_alias
 * area:  wstring
 * funcs:
 * procs: wcslwr wcsupr wcsrev
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs wcslwr, wcsupr, wcsrev (deprecated POSIX names of the underscore functions)
 * note:  PROBE: are the un-prefixed names real DLL exports? Expected yes (mingw import libs list them for ucrtbase and msvcr*).
 *        Each `exp_*` case is the probe; behaviour cases skip when the export is absent.
 *        ucrtbase (observed Win10 22H2 19041 export table via GetProcAddress): `wcslwr`/`wcsupr`/`wcsrev` are NOT exported, `_wcslwr`/
 *        `_wcsupr`/`_wcsrev` are. The ucrtbase build asserts that and runs the behaviour cases against the underscore names; other CRTs
 *        keep the original expectation [unverified there].
 */
#include "tc.h"
#include <wchar.h>

typedef wchar_t *(__cdecl *str1_t)(wchar_t *);
static str1_t p_lwr, p_upr, p_rev;

#if TC_AT_LEAST(140)   /* ucrtbase */
#define U_ "_"
#define ABSENT(n) CHECK(tc_crt_proc(n) == NULL)
#else
#define U_ ""
#define ABSENT(n) do { } while (0)
#endif
TC_CASE(exp_wcslwr) { ABSENT("wcslwr"); CHECK(p_lwr != NULL); }
TC_CASE(exp_wcsupr) { ABSENT("wcsupr"); CHECK(p_upr != NULL); }
TC_CASE(exp_wcsrev) { ABSENT("wcsrev"); CHECK(p_rev != NULL); }
TC_CASE(beh_wcslwr) { wchar_t b[8] = L"AbC1"; if (!p_lwr) TC_SKIP("absent"); CHECK_EQ_P(p_lwr(b), b); CHECK_WSTR(b, L"abc1"); }
TC_CASE(beh_wcsupr) { wchar_t b[8] = L"aBc1"; if (!p_upr) TC_SKIP("absent"); CHECK_EQ_P(p_upr(b), b); CHECK_WSTR(b, L"ABC1"); }
TC_CASE(beh_wcsrev) { wchar_t b[8] = L"abcd"; if (!p_rev) TC_SKIP("absent"); CHECK_EQ_P(p_rev(b), b); CHECK_WSTR(b, L"dcba"); }
int main(void)
{
    tc_begin();
        p_lwr = (str1_t)tc_crt_proc(U_ "wcslwr"); p_upr = (str1_t)tc_crt_proc(U_ "wcsupr"); p_rev = (str1_t)tc_crt_proc(U_ "wcsrev");
    TC_RUN(exp_wcslwr); TC_RUN(exp_wcsupr); TC_RUN(exp_wcsrev);
    TC_RUN(beh_wcslwr); TC_RUN(beh_wcsupr); TC_RUN(beh_wcsrev);
    return tc_end();
}
