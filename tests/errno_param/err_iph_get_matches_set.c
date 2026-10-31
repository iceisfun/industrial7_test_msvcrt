/* TC
 * id:    err_iph_get_matches_set
 * area:  errno_param
 * funcs: _set_invalid_parameter_handler _get_invalid_parameter_handler
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_invalid_parameter_handler
 */
#include "tc.h"
#include <stdlib.h>

static void __cdecl h1(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; }

TC_CASE(get_set)
{
    _invalid_parameter_handler orig = _get_invalid_parameter_handler();
    _invalid_parameter_handler volatile a = h1;
    _set_invalid_parameter_handler(a);
    CHECK(_get_invalid_parameter_handler() == h1);
    CHECK(_get_invalid_parameter_handler() == h1);   /* idempotent */
    _set_invalid_parameter_handler(NULL);
    CHECK(_get_invalid_parameter_handler() == NULL);
    CHECK(_get_invalid_parameter_handler() == NULL);
    _set_invalid_parameter_handler(orig);
    CHECK(_get_invalid_parameter_handler() == orig);
}
int main(void) { tc_begin(); TC_RUN(get_set); return tc_end(); }
