/* TC
 * id:    sec_iph_get_msvcr90_probe
 * area:  secure_crt
 * funcs: _set_invalid_parameter_handler _get_invalid_parameter_handler
 * crt:   msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_invalid_parameter_handler
 * note:  PROBE: whether msvcr90.dll exports _get_invalid_parameter_handler (mingw's import library says yes; believed
 *        introduced with VS2010 otherwise) and, if so, that it returns the handler last installed.
 */
#include "tc.h"
#include <stdlib.h>

static void __cdecl h1(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
TC_CASE(get_returns_installed)
{
    _invalid_parameter_handler orig = _set_invalid_parameter_handler((_invalid_parameter_handler)h1);
    CHECK(_get_invalid_parameter_handler() == (_invalid_parameter_handler)h1);
    _set_invalid_parameter_handler(orig);
}
int main(void) { tc_begin(); TC_RUN(get_returns_installed); return tc_end(); }
