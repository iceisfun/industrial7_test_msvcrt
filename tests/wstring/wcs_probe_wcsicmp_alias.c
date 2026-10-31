/* TC
 * id:    wcs_probe_wcsicmp_alias
 * area:  wstring
 * funcs:
 * procs: wcsicmp wcsnicmp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs wcsicmp, wcsnicmp (deprecated POSIX names of the underscore functions)
 * note:  PROBE: are the un-prefixed names real DLL exports? Expected yes (mingw import libs list them for ucrtbase and msvcr*).
 *        Each `exp_*` case is the probe; behaviour cases skip when the export is absent.
 */
#include "tc.h"
#include <wchar.h>

typedef int (__cdecl *cmp_t)(const wchar_t *, const wchar_t *);
typedef int (__cdecl *ncmp_t)(const wchar_t *, const wchar_t *, size_t);
static cmp_t p_icmp; static ncmp_t p_nicmp;

TC_CASE(exp_wcsicmp) { CHECK(p_icmp != NULL); }
TC_CASE(exp_wcsnicmp) { CHECK(p_nicmp != NULL); }
TC_CASE(beh_wcsicmp) { if (!p_icmp) TC_SKIP("absent"); CHECK_EQ_I(p_icmp(L"AbC", L"aBc"), 0); CHECK(p_icmp(L"a", L"B") < 0); CHECK(p_icmp(L"b", L"A") > 0); }
TC_CASE(beh_wcsnicmp) { if (!p_nicmp) TC_SKIP("absent"); CHECK_EQ_I(p_nicmp(L"ABCx", L"abcy", 3), 0); CHECK(p_nicmp(L"ABCx", L"abcy", 4) < 0); }
int main(void)
{
    tc_begin();
    p_icmp = (cmp_t)tc_crt_proc("wcsicmp"); p_nicmp = (ncmp_t)tc_crt_proc("wcsnicmp");
    TC_RUN(exp_wcsicmp); TC_RUN(exp_wcsnicmp);
    TC_RUN(beh_wcsicmp); TC_RUN(beh_wcsnicmp);
    return tc_end();
}
