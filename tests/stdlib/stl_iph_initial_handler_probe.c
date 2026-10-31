/* TC
 * id:    stl_iph_initial_handler_probe
 * area:  stdlib
 * funcs: _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler
 * note:  Probes that no invalid-parameter handler is installed initially (first setter call returns NULL).
 */
#include "tc.h"
#include <stdlib.h>

typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; }
TC_CASE(initial_null) { CHECK(_set_invalid_parameter_handler((ih_t)h) == NULL); }
int main(void) { tc_begin(); TC_RUN(initial_null); return tc_end(); }
