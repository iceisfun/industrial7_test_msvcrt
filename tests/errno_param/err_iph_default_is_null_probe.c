/* TC
 * id:    err_iph_default_is_null_probe
 * area:  errno_param
 * funcs: _get_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_invalid_parameter_handler
 * note:  PROBE: in a freshly started process (no user handler) the getter returns NULL. Win10 22H2 19045 observation: with the
 *        mingw-w64 test exe the getter returns a handler inside the exe image (installed by the mingw crt startup), so the case
 *        is skipped there; missing datum: the value in an MSVC-built process (class G).
 */
#include "tc.h"
#include <stdlib.h>
TC_CASE(initial)
{
    _invalid_parameter_handler g = _get_invalid_parameter_handler();
    /* see nfx_iph_initial_null: the mingw startup code installs a handler located inside this exe */
    if (g != NULL && tc_addr_in_exe((const void *)g)) TC_SKIP("startup handler installed by the mingw crt (class G: need an MSVC-built exe)");
    CHECK(g == NULL);
}
int main(void) { tc_begin(); TC_RUN(initial); return tc_end(); }
