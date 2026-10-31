/* TC
 * id:    ucr_acmdln_wcmdln_probe
 * area:  ucrt_only
 * funcs: __p__acmdln __p__wcmdln
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   undocumented _acmdln/_wcmdln variables
 * note:  PROBE: the CRT's _acmdln/_wcmdln strings equal GetCommandLineA/W (CRT initialises them from the process command line).
 */
#include "tc.h"

extern char **__cdecl __p__acmdln(void);
extern wchar_t **__cdecl __p__wcmdln(void);

TC_CASE(narrow)
{
    char **p = __p__acmdln();
    CHECK(p != NULL);
    if (p) { CHECK(*p != NULL); CHECK_STR(*p, GetCommandLineA()); }
}
TC_CASE(wide)
{
    wchar_t **p = __p__wcmdln();
    CHECK(p != NULL);
    if (p) { CHECK(*p != NULL); CHECK_WSTR(*p, GetCommandLineW()); }
}
int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
