/* TC
 * id:    ucr_pgmptr_wide_probe
 * area:  ucrt_only
 * funcs: _get_wpgmptr
 * procs: __p__wpgmptr
 * crt:   win10
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _wpgmptr, _get_wpgmptr
 * note:  PROBE: in a program with a narrow main(), _wpgmptr is also initialised to the full module path
 *        (it may be NULL on real Windows if only the narrow variant is set up by startup). Not asserted elsewhere.
 * note:  OBSERVED (Win10 22H2 19045; raw "_get_wpgmptr(&p) == 0 got=22", "p != NULL" failed): in a program with a narrow main()
 *        _wpgmptr is NULL and _get_wpgmptr returns EINVAL. Class A. (mod[] unused now; the wide-main case is not built here.)
 */
#include "tc.h"
#include <stdlib.h>
typedef wchar_t **(__cdecl *wpp_t)(void);
TC_CASE(wide)
{
    wchar_t *p = (wchar_t *)1, mod[MAX_PATH]; wpp_t wpp = (wpp_t)tc_crt_proc("__p__wpgmptr");
    GetModuleFileNameW(NULL, mod, MAX_PATH);
    /* observed Win10 22H2: narrow main -> _wpgmptr is NULL, _get_wpgmptr returns EINVAL(22) and leaves p untouched */
    tc_iph_count = 0;
    CHECK_EQ_I(_get_wpgmptr(&p), 22);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK(p == (wchar_t *)1);   /* untouched */
    if (wpp) CHECK(*wpp() == NULL);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(wide); return tc_end(); }
