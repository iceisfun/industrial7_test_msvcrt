/* TC
 * id:    stl_iph_initial_handler_probe
 * area:  stdlib
 * funcs: _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler
 * note:  Probes that no invalid-parameter handler is installed initially (first setter call returns NULL).
 *        Win10 22H2 19045 (observed): with the mingw-w64 built test exe the first _set_invalid_parameter_handler call returns a handler inside the exe image
 *        (installed by the mingw crt startup), so the case is skipped there. Missing datum: the value in an MSVC-built process (class G).
 */
#include "tc.h"
#include <stdlib.h>

typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; }
TC_CASE(initial_null)
{
    ih_t old = _set_invalid_parameter_handler((ih_t)h);
    /* the mingw-w64 crt startup installs a handler located in the test exe; the value in an MSVC-built process is not known (class G) */
    if (old != NULL && tc_addr_in_exe((const void *)old)) TC_SKIP("startup handler installed by the mingw crt (class G: need an MSVC-built exe)");
    CHECK(old == NULL);
}
int main(void) { tc_begin(); TC_RUN(initial_null); return tc_end(); }
