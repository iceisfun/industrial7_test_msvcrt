/* TC
 * id:    nfx_iph_initial_null
 * area:  negative_fatal
 * kind:  positive
 * funcs: _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler
 * note:  PROBE: a fresh process has no invalid-parameter handler (the first set returns NULL). Win10 22H2 19045 observation:
 *        with the mingw-w64 test exe the first set returns a handler located inside the exe image (installed by the mingw crt
 *        startup), so the case is skipped there; the datum still missing is the value in an MSVC-built process (class G).
 */
#include "tc.h"
#include <stdlib.h>
static void __cdecl h(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
TC_CASE(initial_null)
{
    _invalid_parameter_handler prev = _set_invalid_parameter_handler(h);
    _set_invalid_parameter_handler(prev);
    /* mingw-w64's crt startup installs its own handler (an address inside THIS exe); that is the observed Win10 value, and it says nothing
     * about a fresh MSVC-built process. Missing datum: the same probe from an MSVC-built exe. */
    if (prev != NULL && tc_addr_in_exe((const void *)prev)) TC_SKIP("startup handler installed by the mingw crt (class G: need an MSVC-built exe)");
    CHECK_EQ_P(prev, NULL);
}
int main(void) { tc_begin(); TC_RUN(initial_null); return tc_end(); }
