/* TC
 * id:    err_iph_set_returns_previous
 * area:  errno_param
 * funcs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler: returns the previous handler
 */
#include "tc.h"
#include <stdlib.h>

static void __cdecl h1(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; }
static void __cdecl h2(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; }

TC_CASE(chain)
{
    _invalid_parameter_handler volatile a = h1, b = h2;
    _invalid_parameter_handler orig = _set_invalid_parameter_handler(a);
    _invalid_parameter_handler r2 = _set_invalid_parameter_handler(b);
    _invalid_parameter_handler r3 = _set_invalid_parameter_handler(NULL);
    _invalid_parameter_handler r4;
    CHECK(r2 == h1);
    CHECK(r3 == h2);
    r4 = _set_invalid_parameter_handler(orig);
    CHECK(r4 == NULL);
    CHECK(_set_invalid_parameter_handler(orig) == orig);
}
int main(void) { tc_begin(); TC_RUN(chain); return tc_end(); }
