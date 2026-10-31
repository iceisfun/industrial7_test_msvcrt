/* TC
 * id:    nfx_iph_initial_null
 * area:  negative_fatal
 * kind:  positive
 * funcs: _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler
 * note:  Probes that a fresh process has no invalid-parameter handler installed (the first set returns NULL).
 */
#include "tc.h"
#include <stdlib.h>
static void __cdecl h(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }
TC_CASE(initial_null) { _invalid_parameter_handler prev = _set_invalid_parameter_handler(h); CHECK_EQ_P(prev, NULL); _set_invalid_parameter_handler(prev); }
int main(void) { tc_begin(); TC_RUN(initial_null); return tc_end(); }
