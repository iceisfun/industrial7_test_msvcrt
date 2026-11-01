/* TC
 * id:    dat_wpgmptr_get_basic
 * area:  data_exports
 * kind:  positive
 * funcs: _get_wpgmptr __p__wpgmptr
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _wpgmptr (documented for wmain programs)
 * note:  PROBE: in a narrow main() program _wpgmptr may or may not be populated. Asserts only return code 0, and if non-NULL, basename equality with GetModuleFileNameW.
 * note:  OBSERVED (Win10 22H2 19045): in a narrow main() program _get_wpgmptr(&p) returns EINVAL (22) and leaves p untouched
 *        (raw: "_get_wpgmptr(&p) == 0 got=22", "p != 1" failed). Class A.
 */
#include "tc.h"

#include <stdlib.h>
#ifndef TC_UCRT
extern wchar_t **__cdecl __p__wpgmptr(void);
#endif

static wchar_t lcw(wchar_t c) { return (c >= L'A' && c <= L'Z') ? (wchar_t)(c + 32) : c; }
static const wchar_t *wbase(const wchar_t *s) { const wchar_t *b = s; for (; *s; s++) if (*s == L'\\' || *s == L'/') b = s + 1; return b; }

TC_CASE(returns_zero_or_ptr_set)
{
    /* observed Win10 22H2: in a narrow-main program _wpgmptr is NULL and _get_wpgmptr fails with EINVAL, leaving *p untouched */
    wchar_t *p = (wchar_t *)1; int r;
    tc_iph_count = 0; r = _get_wpgmptr(&p);
    CHECK_EQ_I(r, 22);
    CHECK_EQ_I(tc_iph_count, 1);   /* EINVAL is reported through the invalid-parameter handler */
    CHECK(p == (wchar_t *)1);
}
TC_CASE(wide_path_matches_GetModuleFileNameW)
{
    wchar_t *p = NULL; wchar_t exe[MAX_PATH]; const wchar_t *a, *b;
    _get_wpgmptr(&p);
    if (!p) TC_SKIP("_wpgmptr NULL in narrow main");
    GetModuleFileNameW(NULL, exe, MAX_PATH);
    a = wbase(p); b = wbase(exe);
    while (*a && lcw(*a) == lcw(*b)) { a++; b++; }
    CHECK(lcw(*a) == lcw(*b));
}
TC_CASE(same_as_accessor) { wchar_t *p = NULL; wchar_t **a = __p__wpgmptr(); _get_wpgmptr(&p); CHECK(a != NULL); if (a) CHECK_EQ_P(*a, p); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(same_as_accessor); TC_RUN(returns_zero_or_ptr_set); TC_RUN(wide_path_matches_GetModuleFileNameW); return tc_end(); }
