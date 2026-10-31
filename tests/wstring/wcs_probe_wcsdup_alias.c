/* TC
 * id:    wcs_probe_wcsdup_alias
 * area:  wstring
 * funcs:
 * procs: wcsdup
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs wcsdup (deprecated POSIX name of _wcsdup; "oldnames" in the static libs)
 * note:  PROBE: is the un-prefixed name `wcsdup` an actual DLL export? Expected yes in msvcrt/msvcr*, and (per the mingw
 *        import lib) ucrtbase. Case `exported` is the probe, `works` only runs when the export exists.
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>

typedef wchar_t *(__cdecl *wcsdup_t)(const wchar_t *);
static wcsdup_t p;
TC_CASE(exported) { CHECK(p != NULL); }
TC_CASE(works)
{
    wchar_t *d; static const wchar_t s[] = L"dup me";
    if (!p) TC_SKIP("wcsdup not exported");
    d = p(s); CHECK(d != NULL && d != s); if (d) CHECK_WSTR(d, L"dup me");
    free(d);
}
int main(void) { tc_begin(); p = (wcsdup_t)tc_crt_proc("wcsdup"); TC_RUN(exported); TC_RUN(works); return tc_end(); }
