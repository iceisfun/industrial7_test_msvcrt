/* TC
 * id:    ucr_pgmptr_wide_probe
 * area:  ucrt_only
 * funcs: _get_wpgmptr
 * procs: __p__wpgmptr
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _wpgmptr, _get_wpgmptr
 * note:  PROBE: in a program with a narrow main(), _wpgmptr is also initialised to the full module path
 *        (it may be NULL on real Windows if only the narrow variant is set up by startup). Not asserted elsewhere.
 */
#include "tc.h"
#include <stdlib.h>
typedef wchar_t **(__cdecl *wpp_t)(void);
TC_CASE(wide)
{
    wchar_t *p = NULL, mod[MAX_PATH]; wpp_t wpp = (wpp_t)tc_crt_proc("__p__wpgmptr");
    GetModuleFileNameW(NULL, mod, MAX_PATH);
    CHECK_EQ_I(_get_wpgmptr(&p), 0);
    CHECK(p != NULL);
    if (p) CHECK_WSTR(p, mod);
    if (wpp) CHECK_EQ_P(*wpp(), p);
}
int main(void) { tc_begin(); TC_RUN(wide); return tc_end(); }
