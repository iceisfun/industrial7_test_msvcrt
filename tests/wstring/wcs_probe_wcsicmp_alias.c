/* TC
 * id:    wcs_probe_wcsicmp_alias
 * area:  wstring
 * funcs:
 * procs: wcsicmp wcsnicmp
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs wcsicmp, wcsnicmp (deprecated POSIX names of the underscore functions)
 * note:  PROBE: are the un-prefixed names real DLL exports? Expected yes (mingw import libs list them for ucrtbase and msvcr*).
 *        Each `exp_*` case is the probe; behaviour cases skip when the export is absent.
 *        ucrtbase (observed Win10 22H2 19041 export table via GetProcAddress): `wcsicmp`/`wcsnicmp` are NOT exported, `_wcsicmp`/
 *        `_wcsnicmp` are. The ucrtbase build asserts that and runs the behaviour cases against the underscore names; other CRTs keep
 *        the original expectation [unverified there].
 */
#include "tc.h"
#include <wchar.h>

typedef int (__cdecl *cmp_t)(const wchar_t *, const wchar_t *);
typedef int (__cdecl *ncmp_t)(const wchar_t *, const wchar_t *, size_t);
static cmp_t p_icmp; static ncmp_t p_nicmp;

#if TC_AT_LEAST(140)   /* ucrtbase */
#define N_ICMP "_wcsicmp"
#define N_NICMP "_wcsnicmp"
#define ABSENT(n) CHECK(tc_crt_proc(n) == NULL)
#else
#define N_ICMP "wcsicmp"
#define N_NICMP "wcsnicmp"
#define ABSENT(n) do { } while (0)
#endif
TC_CASE(exp_wcsicmp) { ABSENT("wcsicmp"); CHECK(p_icmp != NULL); }
TC_CASE(exp_wcsnicmp) { ABSENT("wcsnicmp"); CHECK(p_nicmp != NULL); }
TC_CASE(beh_wcsicmp) { if (!p_icmp) TC_SKIP("absent"); CHECK_EQ_I(p_icmp(L"AbC", L"aBc"), 0); CHECK(p_icmp(L"a", L"B") < 0); CHECK(p_icmp(L"b", L"A") > 0); }
TC_CASE(beh_wcsnicmp) { if (!p_nicmp) TC_SKIP("absent"); CHECK_EQ_I(p_nicmp(L"ABCx", L"abcy", 3), 0); CHECK(p_nicmp(L"ABCx", L"abcy", 4) < 0); }
int main(void)
{
    tc_begin();
    p_icmp = (cmp_t)tc_crt_proc(N_ICMP); p_nicmp = (ncmp_t)tc_crt_proc(N_NICMP);
    TC_RUN(exp_wcsicmp); TC_RUN(exp_wcsnicmp);
    TC_RUN(beh_wcsicmp); TC_RUN(beh_wcsnicmp);
    return tc_end();
}
