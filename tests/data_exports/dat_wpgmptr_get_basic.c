/* TC
 * id:    dat_wpgmptr_get_basic
 * area:  data_exports
 * kind:  positive
 * funcs: _get_wpgmptr __p__wpgmptr
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _wpgmptr (documented for wmain programs)
 * note:  PROBE: in a narrow main() program _wpgmptr may or may not be populated. Asserts only return code 0, and if non-NULL, basename equality with GetModuleFileNameW.
 */
#include "tc.h"

#include <stdlib.h>
#ifndef TC_UCRT
extern wchar_t **__cdecl __p__wpgmptr(void);
#endif

static wchar_t lcw(wchar_t c) { return (c >= L'A' && c <= L'Z') ? (wchar_t)(c + 32) : c; }
static const wchar_t *wbase(const wchar_t *s) { const wchar_t *b = s; for (; *s; s++) if (*s == L'\\' || *s == L'/') b = s + 1; return b; }

TC_CASE(returns_zero_or_ptr_set) { wchar_t *p = (wchar_t *)1; CHECK_EQ_I(_get_wpgmptr(&p), 0); CHECK(p != (wchar_t *)1); }
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
int main(void) { tc_begin(); TC_RUN(same_as_accessor); TC_RUN(returns_zero_or_ptr_set); TC_RUN(wide_path_matches_GetModuleFileNameW); return tc_end(); }
