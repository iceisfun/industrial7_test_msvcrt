/* TC
 * id:    wcs_probe_wcsdup_alias
 * area:  wstring
 * funcs:
 * procs: wcsdup
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs wcsdup (deprecated POSIX name of _wcsdup; "oldnames" in the static libs)
 * note:  PROBE: is the un-prefixed name `wcsdup` an actual DLL export? Expected yes in msvcrt/msvcr*, and (per the mingw
 *        import lib) ucrtbase. Case `exported` is the probe, `works` only runs when the export exists.
 *        ucrtbase (observed Win10 22H2 19041 export table via GetProcAddress): `wcsdup` is NOT exported, `_wcsdup` is. The ucrtbase
 *        build asserts that and runs `works` against `_wcsdup`; other CRTs keep the original expectation [unverified there].
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>

typedef wchar_t *(__cdecl *wcsdup_t)(const wchar_t *);
#if TC_AT_LEAST(140)   /* ucrtbase */
#define WCSDUP_NAME "_wcsdup"
#else
#define WCSDUP_NAME "wcsdup"
#endif
static wcsdup_t p;
TC_CASE(exported)
{
#if TC_AT_LEAST(140)   /* ucrtbase */
    CHECK(tc_crt_proc("wcsdup") == NULL); CHECK(tc_crt_proc("_wcsdup") != NULL);
#else
    CHECK(p != NULL);
#endif
}
TC_CASE(works)
{
    wchar_t *d; static const wchar_t s[] = L"dup me";
    if (!p) TC_SKIP(WCSDUP_NAME " not exported");
    d = p(s); CHECK(d != NULL && d != s); if (d) CHECK_WSTR(d, L"dup me");
    free(d);
}
int main(void) { tc_begin(); p = (wcsdup_t)tc_crt_proc(WCSDUP_NAME); TC_RUN(exported); TC_RUN(works); return tc_end(); }
