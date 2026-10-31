/* TC
 * id:    sec_iph_set_null_get_probe
 * area:  secure_crt
 * funcs: _set_invalid_parameter_handler _get_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_invalid_parameter_handler ("null pointer if none has been set")
 * note:  PROBE: after _set_invalid_parameter_handler(NULL), _get_invalid_parameter_handler() returns NULL.
 */
#include "tc.h"
#include <stdlib.h>

static void __cdecl h1(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
TC_CASE(set_null_then_get_returns_null)
{
    _invalid_parameter_handler orig = _set_invalid_parameter_handler((_invalid_parameter_handler)h1);
    _set_invalid_parameter_handler(NULL);
    CHECK(_get_invalid_parameter_handler() == NULL);
    _set_invalid_parameter_handler(orig);
}
int main(void) { tc_begin(); TC_RUN(set_null_then_get_returns_null); return tc_end(); }
