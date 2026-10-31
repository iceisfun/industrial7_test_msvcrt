/* TC
 * id:    sec_iph_set_get
 * area:  secure_crt
 * funcs: _set_invalid_parameter_handler _get_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, _get_invalid_parameter_handler
 * note:  msvcr90 excluded: _get_invalid_parameter_handler is not certain to be exported there (see sec_iph_get_msvcr90_probe).
 *        get-after-set(NULL) lives in sec_iph_set_null_get_probe.
 */
#include "tc.h"
#include <stdlib.h>

typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static void __cdecl h1(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
static void __cdecl h2(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }

TC_CASE(set_returns_previous)
{
    iph_t orig = (iph_t)_set_invalid_parameter_handler((_invalid_parameter_handler)h1);
    iph_t prev = (iph_t)_set_invalid_parameter_handler((_invalid_parameter_handler)h2);
    CHECK(prev == h1);
    _set_invalid_parameter_handler((_invalid_parameter_handler)orig);
}
TC_CASE(get_returns_installed)
{
    iph_t orig = (iph_t)_set_invalid_parameter_handler((_invalid_parameter_handler)h1);
    CHECK((iph_t)_get_invalid_parameter_handler() == h1);
    _set_invalid_parameter_handler((_invalid_parameter_handler)h2);
    CHECK((iph_t)_get_invalid_parameter_handler() == h2);
    _set_invalid_parameter_handler((_invalid_parameter_handler)orig);
}
TC_CASE(restore_original)
{
    iph_t orig = (iph_t)_set_invalid_parameter_handler((_invalid_parameter_handler)h1);
    _set_invalid_parameter_handler((_invalid_parameter_handler)orig);
    CHECK((iph_t)_get_invalid_parameter_handler() == orig);
}
int main(void)
{
    tc_begin(); TC_RUN(set_returns_previous); TC_RUN(get_returns_installed);
    TC_RUN(restore_original);
    return tc_end();
}
