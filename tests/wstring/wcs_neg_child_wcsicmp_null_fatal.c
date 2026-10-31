/* TC
 * id:    wcs_neg_child_wcsicmp_null_fatal
 * area:  wstring
 * funcs: _wcsicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _stricmp, _wcsicmp; Parameter validation (default handler terminates the program)
 */
#include "tc.h"
#include <wchar.h>

static const wchar_t *volatile vs = L"abc", *volatile vn = NULL;

TC_CASE(child_first_null) { DWORD code = 0; int ran = tc_spawn_self("first", &code, 20000); CHECK(ran); CHECK(code != 77); CHECK(code != 0); }
TC_CASE(child_second_null) { DWORD code = 0; int ran = tc_spawn_self("second", &code, 20000); CHECK(ran); CHECK(code != 77); CHECK(code != 0); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'f') _wcsicmp(vn, vs); else _wcsicmp(vs, vn);
        return 77;
    }
    TC_RUN(child_first_null); TC_RUN(child_second_null);
    return tc_end();
}
